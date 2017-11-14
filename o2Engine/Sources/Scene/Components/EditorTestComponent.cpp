#include "EditorTestComponent.h"

#include "Scene/Actor.h"

#include "Scene/SceneLayer.h"

DECLARE_CLASS(o2::EditorTestComponent);

DECLARE_CLASS(o2::EditorTestComponent::TestInside);

ENUM_META_(o2::EditorTestComponent::TestEnum, TestEnum)
{
	ENUM_ENTRY(A);
	ENUM_ENTRY(B);
	ENUM_ENTRY(C);
	ENUM_ENTRY(D);
	ENUM_ENTRY(E);
	ENUM_ENTRY(F);
	ENUM_ENTRY(G);
	ENUM_ENTRY(H);
	ENUM_ENTRY(K);
	ENUM_ENTRY(L);
	ENUM_ENTRY(M);
	ENUM_ENTRY(N);
}
END_ENUM_META;
