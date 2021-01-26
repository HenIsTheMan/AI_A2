namespace Obj{
	template <class T, typename Type>
	KnightCreationAttribs<T, Type>::KnightCreationAttribs(const int lvl, const int gridCols, const Sim* const sim, const bool isFlatTop):
		IEntityCreationAttribs<T, Type>()
	{
		IEntityCreationAttribs<T, Type>::im_LocalPos = FindValidLocalPos<T>(gridCols, sim);

		IEntityCreationAttribs<T, Type>::im_Team = sim->turn == SimTurn::Player ? EntityTeam::Player : EntityTeam::AI;
		IEntityCreationAttribs<T, Type>::im_FacingDir = ChooseRandFacingDir(
			isFlatTop,
			(int)IEntityCreationAttribs<T, Type>::im_LocalPos.y,
			(int)IEntityCreationAttribs<T, Type>::im_LocalPos.x
		);
		IEntityCreationAttribs<T, Type>::im_Lvl = lvl;

		switch(lvl){
			case 1:
				IEntityCreationAttribs<T, Type>::im_ActionType = EntityActionType::UnidirectionalAttack;
				IEntityCreationAttribs<T, Type>::im_ActionRange = 1;
				IEntityCreationAttribs<T, Type>::im_VisionType = EntityVisionType::Omnidirectional;
				IEntityCreationAttribs<T, Type>::im_VisionRange = 1;

				IEntityCreationAttribs<T, Type>::im_ActionMag = 1.0f;
				IEntityCreationAttribs<T, Type>::im_Health = 4.0f;
				break;
			case 2:
				IEntityCreationAttribs<T, Type>::im_ActionType = EntityActionType::UnidirectionalAttack;
				IEntityCreationAttribs<T, Type>::im_ActionRange = 1;
				IEntityCreationAttribs<T, Type>::im_VisionType = EntityVisionType::Omnidirectional;
				IEntityCreationAttribs<T, Type>::im_VisionRange = 2;

				IEntityCreationAttribs<T, Type>::im_ActionMag = 2.0f;
				IEntityCreationAttribs<T, Type>::im_Health = 8.0f;
				break;
			case 3:
				IEntityCreationAttribs<T, Type>::im_ActionType = EntityActionType::UnidirectionalAttack;
				IEntityCreationAttribs<T, Type>::im_ActionRange = 1;
				IEntityCreationAttribs<T, Type>::im_VisionType = EntityVisionType::Omnidirectional;
				IEntityCreationAttribs<T, Type>::im_VisionRange = 3;

				IEntityCreationAttribs<T, Type>::im_ActionMag = 3.0f;
				IEntityCreationAttribs<T, Type>::im_Health = 16.0f;
				break;
		}
	}
}