#include "o2/stdafx.h"
#include "EditorTestComponent.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/SceneLayerRef.h"

o2::String o2::EditorTestComponent::GetCategory()
{
    return "Debug";
}

void o2::EditorTestComponent::Test()
{
    //o2Debug.Log("test");
}

void o2::EditorTestComponent::OnStart()
{
    mTestInsideRef = mmake<TestInside>();
}
// --- META ---

ENUM_META(o2::EditorTestComponent::TestEnum)
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

DECLARE_CLASS(o2::EditorTestComponent, o2__EditorTestComponent);

DECLARE_CLASS(o2::EditorTestComponent::TestInside, o2__EditorTestComponent__TestInside);
// --- END META ---
