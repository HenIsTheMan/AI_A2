namespace Obj{
	template <class T, typename Type>
	GunnerCreationAttribs<T, Type>::GunnerCreationAttribs(const int gridCols, const Sim* const sim, const bool isFlatTop):
		IEntityCreationAttribs<T, Type>()
	{
		IEntityCreationAttribs<T, Type>::im_LocalPos = FindValidLocalPos<T>(gridCols, sim);

		IEntityCreationAttribs<T, Type>::im_Team = sim->turn == SimTurn::Player ? EntityTeam::Player : EntityTeam::AI;
		IEntityCreationAttribs<T, Type>::im_FacingDir = ChooseRandFacingDir(
			isFlatTop,
			(int)IEntityCreationAttribs<T, Type>::im_LocalPos.y,
			(int)IEntityCreationAttribs<T, Type>::im_LocalPos.x
		);

		IEntityCreationAttribs<T, Type>::im_ActionType = EntityActionType::UnidirectionalAttack;
		IEntityCreationAttribs<T, Type>::im_ActionRange = 2;
		IEntityCreationAttribs<T, Type>::im_VisionType = EntityVisionType::Unidirectional;
		IEntityCreationAttribs<T, Type>::im_VisionRange = 5;

		IEntityCreationAttribs<T, Type>::im_ActionMag = 2.0f;
		IEntityCreationAttribs<T, Type>::im_Health = 4.0f;
		IEntityCreationAttribs<T, Type>::im_Spd = 12.0f;
	}
}