
#include "wobjs/RXTree.h"

namespace qkc {

	RadixTree::RadixTree()
	{
		mmgr_ = NULL;
		max_bits_ = 0;
		bits_ = NULL;
		level_ = 0;
		root_ = NULL;
	}

	RadixTree::~RadixTree()
	{
		Final();
	}

	bool RadixTree::Init(int max_bits, int level, MemMgr * mmgr)
	{
		if (level <= 0 || max_bits <= level)
			return false;

		if (mmgr == NULL)
			mmgr_ = MemMgr::Default();
		else
			mmgr_ = mmgr;

		//����ÿһ��ı���λ������Ҫע����ǣ����һ�㲻һ�������õ�
		int expect_bits = (max_bits + level - 1) / level ;
		if (expect_bits > kMaxBitCap)
		{
			//�����������ı���λ�����¼������
			level = (max_bits + kMaxBitCap - 1) / kMaxBitCap;
			expect_bits = (max_bits + level - 1) / level;
		}

		//�������λ����
		int * bits = (int *)mmgr_->Alloc(sizeof(int) * level, true);

		//��ֵ����
		for (int lvl = 0; lvl < level - 1; ++lvl)
		{
			bits[lvl] = expect_bits;
		}
		//��ֵҶ�ӽڵ�
		bits[level - 1] = max_bits - expect_bits * (level - 1);

		bits_ = bits;
		level_ = level;
		max_bits_ = max_bits;

		size_t rsize = sizeof(uintptr_t) * (1 << bits_[0]);
		root_ = (uintptr_t *)mmgr_->Alloc(rsize , true);
		return (root_ != NULL);
	}

	typedef struct __st_travel_node {
		uintptr_t *ptr;
		int offset;
	} travel_node_t;

	void RadixTree::Final()
	{
		travel_node_t nodes[64] = { root_ , 0 };

		int lvl = 0;
		while (lvl >= 0)
		{
			uintptr_t * arr = nodes[lvl].ptr;
			if (arr != NULL)
			{
				//�������±���
				int ofs = nodes[lvl].offset;
				int lvl_size = 1 << bits_[lvl];
				uintptr_t addr = 0;
				if (lvl + 1 < level_)
				{
					//�������һ��
					while (ofs < lvl_size)
					{
						addr = arr[ofs];
						if (addr != 0)
							break;
						++ofs;
					}
				}

				if (addr != 0)
				{
					//�ҵ���һ��ڵ�����
					++lvl;
					nodes[lvl].ptr = (uintptr_t *)addr;
					nodes[lvl].offset = 0;
				}
				else
				{
					//û���ҵ���һ�㣬��ɾ������
					nodes[lvl].ptr = NULL;
					nodes[lvl].offset = 0;
					mmgr_->Free(arr);
				}
			}//end if (arr != NULL)
			else
			{
				--lvl;
				if (lvl < 0)
					break;

				nodes[lvl].offset++;
			}
		}//end while (lvl >= 0)
	}

	int RadixTree::CalcPath(uintptr_t key, uintptr_t * path, int count) const
	{
		if (count < level_)
			return -1;

		int max_bits = max_bits_;

		for (int lvl = 0; lvl < level_; ++lvl)
		{
			//����key�ڱ����ƫ����
			int cur_bits = bits_[lvl];
			max_bits -= cur_bits;

			path[lvl] = (key >> max_bits) & CalcMask(cur_bits);
		}

		return level_;
	}

	int RadixTree::CalcPath(uintptr_t * path, int count, uintptr_t * nodes) const
	{
		if (path == NULL || count < level_ || nodes == NULL)
			return -1;

		nodes[0] = (uintptr_t)root_;
		int lvl = 0;
		while(lvl < level_)
		{
			uintptr_t offset = path[lvl];
			uintptr_t * narr = (uintptr_t *)nodes[lvl];
			if (narr == NULL)
				break;

			++lvl;			
			nodes[lvl] = narr[offset];
		}

		return lvl;
	}


	bool RadixTree::Insert(uintptr_t key, uintptr_t value)
	{
		uintptr_t path[kMaxLevel] = { 0 };
		if (CalcPath(key, path, kMaxLevel) <= 0)
			return false;

		uintptr_t * nodes = root_;

		for (int lvl = 0; lvl < level_ -1 ; ++lvl)
		{
			uintptr_t offset = path[lvl];

			//������һ������
			uintptr_t narr = nodes[offset];
			if (narr == 0)
			{
				//��û�з����ڴ��
				int nsize = sizeof(uintptr_t) * (1 << bits_[lvl]);
				narr = (uintptr_t)mmgr_->Alloc(nsize, true);
				nodes[offset] = narr;
			}
			nodes = (uintptr_t *)narr;
		}

		nodes[path[level_ - 1]] = value;
		return true;
	}

	bool RadixTree::Find(uintptr_t key, uintptr_t& value)const
	{
		uintptr_t path[kMaxLevel] = { 0 };
		if (CalcPath(key, path, kMaxLevel) <= 0)
			return false;

		uintptr_t * nodes = root_;
		for (int lvl = 0; lvl < level_ - 1; ++lvl)
		{
			uintptr_t offset = path[lvl];

			uintptr_t narr = nodes[offset];
			if (narr == 0)
				return false;

			nodes = (uintptr_t *)narr;
		}

		value = nodes[path[level_ - 1]];
		return true;
	}

	bool RadixTree::Delete(uintptr_t key, uintptr_t& value)
	{
		uintptr_t path[kMaxLevel] = { 0 };
		int count = CalcPath(key, path, kMaxLevel);
		if(count <= 0)
			return false;

		uintptr_t nodes[kMaxLevel] = { 0 };
		
		int nsize = CalcPath(path, count, nodes);
		if (nsize != count)
			return false;

		uintptr_t offset = path[nsize - 1];
		uintptr_t * narr = (uintptr_t *)nodes[nsize - 1];
		if (narr == NULL)
			return false;

		value = narr[offset];
		narr[offset] = 0;
		return true;
	}

}
