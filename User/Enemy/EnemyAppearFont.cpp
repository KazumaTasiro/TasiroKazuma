#include "EnemyAppearFont.h"

void EnemyAppearFont::Initialize(Object3d* boss)
{
	assert(boss);
	fontModel_[ 0 ] = Model::LoadFormOBJ("bo");
	fontModel_[ 1 ] = Model::LoadFormOBJ("su");
	fontModel_[ 2 ] = Model::LoadFormOBJ("saka");
	fontModel_[ 3 ] = Model::LoadFormOBJ("bann");
	for ( int i = 0; i < 4; i++ )
	{
		fontWorldTransform_[ i ] = new Object3d();
		fontWorldTransform_[ i ]->Initialize();
		fontWorldTransform_[ i ]->SetModel(fontModel_[ i ]);
		fontWorldTransform_[ i ]->SetParent(boss);
		fontWorldTransform_[ i ]->wtf.scale = { 10,10,10 };
	}
}

void EnemyAppearFont::Update()
{

	for ( int i = 0; i < 4; i++ )
	{
		fontWorldTransform_[ i ]->Update();
	}
}

void EnemyAppearFont::Draw()
{
	for ( int i = 0; i < 4; i++ )
	{
		fontWorldTransform_[ i ]->Draw();
	}
}

void EnemyAppearFont::OnCollision()
{

}

Vector3 EnemyAppearFont::GetWorldPosition(int nmb)
{
	return fontWorldTransform_[ nmb ]->wtf.position;
}

void EnemyAppearFont::Attck()
{
	if ( nowAttck == true )
	{

	}
}
