#include <memory>

using namespace o2;

namespace Editor
{
	class IActorHeaderViewer : public IObject
	{
	public:
		virtual ~IActorHeaderViewer() {}

		virtual void SetTargetActors(const Vector<Ref<Actor>>& actors) {}

		virtual Ref<Widget> GetWidget() const { return nullptr; }

		virtual void Refresh() {}

		void SetEnabled(bool enabled);

		bool IsEnabled() const;

		IOBJECT(IActorHeaderViewer);

	protected:
		bool mEnabled = false;

	protected:
		virtual void OnEnabled() {}

		virtual void OnDisabled() {}

		friend class ActorViewer;
	};
}

CLASS_BASES_META(Editor::IActorHeaderViewer)
{
    BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IActorHeaderViewer)
{
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mEnabled);
}
END_META;
CLASS_METHODS_META(Editor::IActorHeaderViewer)
{
    FUNCTION().PUBLIC().SIGNATURE(void, SetTargetActors, const Vector<Ref<Actor>>&);
    FUNCTION().PUBLIC().SIGNATURE(Ref<Widget>, GetWidget);
    FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
    FUNCTION().PUBLIC().SIGNATURE(void, SetEnabled, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsEnabled);
    FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
}
END_META;