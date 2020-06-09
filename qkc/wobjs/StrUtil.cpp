
#include "wobjs/StrUtil.h"

namespace qkc {


	size_t StrSplit(const char * str, const char  seperator, StrSeg * segs, size_t count)
	{
		if (str == NULL)
			return 0;

		size_t size = 0, offset = 0;
		const char * start = str;
		for (int idx = 0; ; ++idx)
		{
			char ch = str[idx];
			if (ch == seperator || ch == '\0')
			{
				if (offset >= count)
					break;

				if (size != 0)
				{
					segs[offset].Start = start;
					segs[offset].Size = size;
					++offset;
				}
				if (ch == '\0')
					break;

				start = str + idx + 1;
				size = 0;
			}
			else
				++size;
		}

		return offset;
	}

	int StrNCmp(const char * src, size_t nsize, const char * dst)
	{
		if (src == NULL)
		{
			if (dst == NULL)
				return 0;
			else
				return -1;
		}
		if (dst == NULL)
			return 1;

		for (size_t idx = 0; idx < nsize; ++idx)
		{
			char ch = src[idx] - dst[idx];
			if (ch != 0)
				return ch;

			if (dst[idx] == '\0')
				return 0;
		}

		if (dst[nsize] == '\0')
			return 0;
		else
			return -1;
	}

	bool StrToInt32(const char * str, int32_t& value)
	{
		int64_t tmp = 0;
		if (StrToInt64(str, tmp) == false)
			return false;

		if (tmp < INT32_MIN ||  tmp > INT32_MAX)
			return false;

		value = (int32_t)tmp;
		return true;	
	}

	bool StrToInt64(const char * str, int64_t& value)
	{
		value = 0;
		if (str == 0)
			return false;

		int offset = 0;
		while (true)
		{
			char ch = str[offset];
			if (ch < '0' || ch > '9')
				break;

			int dec = ch - '0';
			value = value * 10 + dec;
			++offset;
		}
		return true;
	}

	int StrToDate(const char * str, int& year, int& month, int& day)
	{
		year = month = day = 0;
		if (str == NULL)
			return -1;

		char vstr[256] = { '\0' };
		int vsize = 0, stage = 0, offset = 0;

		//1����ȥ���ո�
		while (str[offset] == ' ') offset++;

		//2����������
		bool completed = false;
		while (completed == false && stage < 2)
		{
			int curs = stage;
			char ch = str[offset];
			if (ch == '\0')
			{
				//������ֵ
				completed = true;
				++curs;
			}
			else if (ch == '-' || ch == '/')
			{
				//һ���ڵĽ���
				vstr[vsize] = '\0';
				++curs;
			}
			else if (ch >= '0' && ch <= '9')
			{
				vstr[vsize] = ch;
				++vsize;
			}
			else
			{
				//���ַǷ��ַ�
				return -1;
			}

			//��״̬û�з����ı䣬�ж��ֽ����Ƿ��Ѿ��㹻��
			if (curs == stage)
			{
				bool full = false;
				if (stage == 0 && vsize > 4)		//���
					full = true;
				else if (stage == 1 && vsize > 2)	//�·�
					full = true;
				else if (stage == 2 && vsize > 2)	//����
					full = true;

				if (full == true)
				{
					++curs;
					--offset;//�ع�һ���ֽ�
					--vsize;
				}
			}

			//״̬�����仯�������ֵ
			if (curs != stage)
			{
				vstr[vsize] = '\0';
				vsize = 0;

				int32_t value = 0;
				if (StrToInt32(vstr, value) == false)
					return false;

				if (stage == 0)
					year = value;
				else if (stage == 1)
					month = value;
				else if (stage == 2)
					day = value;

				stage = curs;
			}

			++offset;
		}

		return offset;
	
	}

	int StrToTime(const char * str, int& hour, int& minute, int& second, int& usec)
	{
		if (str == NULL)
			return -1;

		char vstr[256] = { '\0' };
		int vsize = 0, stage = 0, offset = 0;

		//1����ȥ��ǰ׺�Ŀո�
		while (str[offset] == ' ') ++offset;

		//2������ʱ��
		bool completed = false;
		while (completed == false)
		{
			int curs = stage;
			char ch = str[offset];
			if (ch == '\0')
			{
				//������ֵ
				completed = true;
				++curs;
			}
			else if (ch == ':' || ch == '.')
			{
				//һ���ڵĽ���
				vstr[vsize] = '\0';
				++curs;

				if (ch == ':' && stage >= 2)
					return -1;
				else if (ch == '.' && stage != 2)
					return -1;
			}
			else if (ch >= '0' && ch <= '9')
			{
				vstr[vsize] = ch;
				++vsize;
			}
			else
			{
				//���ַǷ��ַ�
				return false;
			}

			//��״̬û�з����ı䣬�ж��ֽ����Ƿ��Ѿ��㹻��
			if (curs == stage)
			{
				bool full = false;
				if (stage == 0 && vsize > 2)	//Сʱ
					full = true;
				else if (stage == 1 && vsize > 2)	//����
					full = true;
				else if (stage == 2 && vsize > 2)	//��
					full = true;
				else if (stage == 3 && vsize > 6)	//΢��
					full = true;
				
				if (full == true)
				{
					++curs;
					--offset;//�ع�һ���ֽ�
					--vsize;
				}
			}

			//״̬�����仯�������ֵ
			if (curs != stage)
			{
				if (stage == 6)
				{
					//΢����������������룬��Ҫ��ȫ��6λ��
					while (vsize < 6)
						vstr[vsize++] = '0';
				}
				vstr[vsize] = '\0';
				vsize = 0;

				int32_t value = 0;
				if (StrToInt32(vstr, value) == false)
					return false;

				if (stage == 0)
					hour = value;
				else if (stage ==1)
					minute = value;
				else if (stage == 2)
					second = value;
				else if (stage == 3)
					usec = value;

				stage = curs;
			}

			++offset;
		}

		return offset;
	}

	int StrToDateTime(const char * str, int&year, int& month, int& day, int& hour, int& minute, int& second, int& usec)
	{
		int date_offset = StrToDate(str, year, month, day);
		if (date_offset < 0)
			return -1;

		int time_offset = StrToTime(str + date_offset, hour, minute, second,  usec);
		if (time_offset < 0)
			return -1;

		return (date_offset + time_offset);	
	}

}

