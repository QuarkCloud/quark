
#include "wobjs/RingBuffer.h"
#include <errno.h>
#include <string.h>

namespace qkc {

	RingBuffer::RingBuffer(MemMgr * mmgr)
	{
		if (mmgr == NULL)
			mmgr = &MemMgr::Singleton;
		mmgr_ = mmgr;
		buffer_ = NULL;
		size_ = 0;
		head_ = tail_ = 0;
	}
	RingBuffer::~RingBuffer()
	{
		Final();
	}

	bool RingBuffer::Init(size_t size)
	{
		char * buffer = (char *)mmgr_->Alloc(size, true);
		if (buffer == NULL)
		{
			errno = ENOMEM;
			return false;
		}

		buffer_ = buffer;
		size_ = size;
		return true;
	}

	void RingBuffer::Final()
	{
		char * buffer = buffer_;
		buffer_ = NULL;
		if (buffer != NULL)
		{
			mmgr_->Free(buffer);
		}
		size_ = 0;
		head_ = tail_ = 0;
	}

	size_t RingBuffer::WriteStream(const void * buf, size_t size)
	{
		size_t head = head_, tail = tail_, bufsize = size_;
		size_t left_size = 0;
		if (head >= tail)
			left_size = bufsize + tail - head;
		else
			left_size = tail - head;
		--left_size;

		size_t copy_size = (left_size >= size) ? size : left_size;
		if (head > tail)
		{
			//β�����ƣ�ֱ�Ӵ�tail����head
			::memcpy(buffer_ + tail, buf, copy_size);
			tail_ += copy_size;
		}
		else
		{
			//�ֶο���
			size_t cont_size = bufsize - tail;
			if (cont_size > copy_size)
				cont_size = copy_size;
			::memcpy(buffer_ + tail, buf, cont_size);
			size_t broken_size = copy_size - cont_size;
			if (broken_size > 0)
			{
				::memcpy(buffer_, (const char *)buf + cont_size, broken_size);
				tail_ = broken_size;
			}
			else
			{
				tail_ += cont_size;
			}
		}

		return copy_size;
	}

	bool RingBuffer::ReferStream(char *& buf, size_t& size)
	{
		if (head_ <= tail_)
		{
			size = tail_ - head_;
			buf = buffer_ + head_;
			return true;
		}

		size = size_ - head_;
		buf = buffer_ + head_;
		return true;
	}

	bool RingBuffer::MoveSize(size_t size)
	{
		size_t head = head_ + size;
		if (head >= size_)
			head -= size_;

		head_ = head;
		return true;
	}

	size_t RingBuffer::WriteMessage(const void * buf, size_t size)
	{
		if (buf == NULL || size == 0 || size >= 0xFFFF)
			return -1;

		size_t head = head_, tail = tail_, bufsize = size_;
		size_t left_size = bufsize - tail;
		if (left_size < size + 4)
		{
			//ʣ��ռ䲻�㣬�����Ի��ƣ��������׳����⡣
			//ֻ�������е���Ϣ�����ͳ�ȥ������ƫ����
			if (head != tail)
				return -1;
			head = tail = 0;
			left_size = size_;
		}

		char * buffer = buffer_ + tail;
		buffer[0] = 0x55;
		buffer[1] = 0x55;
		::memcpy(buffer + 2, &size, 2);
		::memcpy(buffer + 4, buf, size);

		size_t copy_size = size + 4;
		tail_ += copy_size;
		return copy_size;
	}

	bool RingBuffer::ReferMessage(char *&buf, size_t& size)
	{
		size_t head = head_, tail = tail_, bufsize = size_;
		if (head == tail)
		{
			//�յ�
			buf = NULL;
			size = 0;
			return true;
		}

		char * buffer = buffer_;
		size_t magic_size = 0, idx = head;

		for (; idx < tail; ++idx)
		{
			char ch = buffer[idx];
			if (ch == 0x55)
				++magic_size;
			else if (magic_size != 0)
				magic_size = 0;

			if (magic_size == 2)
				break;
		}
		if (magic_size != 2)
		{
			head_ = tail;
			return false;
		}

		size_t msg_size = 0;
		::memcpy(&msg_size, buffer + idx, 2);
		if ((idx + 2 + msg_size) > tail)
		{
			head_ = idx;  //�Թ�ħ�����������Ϣ��������
			idx -= 2;
			buffer[idx] = -1;    //���ħ��������������±���λ
			buffer[idx + 1] = -1;
			return false;
		}

		buf = buffer + idx + 2;
		size = msg_size;
		return true;
	}
}
