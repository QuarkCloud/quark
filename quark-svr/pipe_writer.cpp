
#include "pipe_writer.h"


PipeWriter::PipeWriter()
{
	sending_index_ = writing_index_ = -1 ;
}

PipeWriter::~PipeWriter()
{
	//
}

PipePage * PipeWriter::PrepareWritePage()
{
	PipePage * result = NULL , * tmp = NULL	;
	guard_.Lock();
	int index = -1;
	if (writing_index_ == -1)
		index = 0;

	tmp = pages_ + index;
	if (tmp->Status() == PipePage::kIdle)
	{
		if (tmp->Full() == false)
		{
			tmp->Status(PipePage::kInWrite);
			result = tmp;
		}
		else
		{
			//已经写满了，则判断是否有下一个空闲内存页
			if (index == 0)
				index = 1;
			else
				index = 0;

			tmp = pages_ + index;
			if (tmp->Status() == PipePage::kIdle && tmp->Full() == false)
			{
				tmp->Status(PipePage::kInWrite);
				result = tmp;
			}
		}
	}

	if (result != NULL && writing_index_ != index)
	{
		writing_index_ = index;
	}

	guard_.Unlock();

	return result;
}

void PipeWriter::CompletedWritePage(PipePage * page)
{
	guard_.Lock();

	if (page != NULL && page->Status() == PipePage::kInWrite)
	{
		page->Status(PipePage::kIdle);
	}

	guard_.Unlock();
}

int PipeWriter::Write(const void * buffer, int buflen)
{
	size_t left = 0;
	PipePage* page = PrepareWritePage();
	if (page != NULL)
	{
		left = page->Left();
		if (buflen < (int)left)
			left = buflen;

		if (left != 0)
		{
			::memcpy(page->Tail(), buffer, left);
			page->Append(left);
		}

		CompletedWritePage(page);
	}

	StartSend();
	return left;
}

void PipeWriter::StartSend()
{
	PipePage * page = NULL;
	int index = writing_index_;

	guard_.Lock();
	page = pages_ + (index & 1);
	if ((page->Status() == PipePage::kIdle) && (page->Empty() == false))
		page->Status(PipePage::kInSend);
	else
		page = NULL;
	guard_.Unlock();

	if (page != NULL)
	{
		::WriteFile(this->Handle(), page->Head(), page->Size(), NULL, &ovlp_);
	}
}

void PipeWriter::Sent()
{
	PipePage * page = NULL;
	int index = sending_index_;
	guard_.Lock();
	if (index >= 0)
	{
		page = pages_ + (index & 1);
		if (page->Status() == PipePage::kInSend)
			page->Status(PipePage::kIdle);
	}
	guard_.Unlock();
}


