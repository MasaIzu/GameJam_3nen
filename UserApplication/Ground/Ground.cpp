#include "Ground.h"
#include <CollisionManager.h>

Ground::Ground(Model* model)
{
	groundModel = model;
}

void Ground::Initialze()
{

	groundTrans.Initialize();
	groundTrans.scale_ = { 30,30,30 };
	groundTrans.TransferMatrix();

	//touchableObject.reset(TouchableObject::Create(groundModel, groundTrans, COLLISION_ATTR_LANDSHAPE));


}

void Ground::Update()
{

}

void Ground::Draw(const ViewProjection& view)
{
	groundModel->Draw(groundTrans, view);
}
