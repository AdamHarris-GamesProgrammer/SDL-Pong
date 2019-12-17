#pragma once

class Component {
public:
	Component(class Actor* owner, int updateOrder = 100) {
		mOwner = owner;
		mUpdateOrder = updateOrder;
	}

	virtual ~Component();


	virtual void Update(float deltaTime);

	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	class Actor* mOwner;

	int mUpdateOrder;
};