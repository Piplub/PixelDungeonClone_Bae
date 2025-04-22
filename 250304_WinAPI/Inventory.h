#pragma once
#include "config.h"

class Item;
class Inventory
{
private:
	// ���� 25���� ���� ��ø -> ���ߺ���? or ���� ���� ����
	vector<Item*> items{ 25, nullptr };
public:
	Inventory();
	~Inventory();

	void AddItem(Item* item);
	Item* GetItemAt(int index);
	// �ܺο��� remove �ϰ� �� ģ���� ó�� �� delete?
	Item* RemoveItemAt(int index);
};

