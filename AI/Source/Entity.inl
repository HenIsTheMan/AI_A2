namespace Obj{
	template <class T, typename Type>
	Entity<T, Type>::Entity():
		Entity(EntityAttribs<T, Type>())
	{
	}

	template <class T, typename Type>
	Entity<T, Type>::Entity(const EntityAttribs<T, Type>& attribs):
		im_Attribs(attribs)
	{
	}

	template <class T, typename Type>
	int Entity<T, Type>::OnEvent(Event* myEvent, const bool destroyEvent){
		if(!myEvent){
			return -1;
		}

		int val = -1;

		switch(myEvent->GetID()){
			case EventID::EventBlockPlaced: {
				const EventBlockPlaced* const eventBlockPlaced = dynamic_cast<const EventBlockPlaced*>(myEvent);
				assert(eventBlockPlaced && "Val of eventBlockPlaced is nullptr!");

				const float blockRow = (float)eventBlockPlaced->GetBlockRow();
				const float blockCol = (float)eventBlockPlaced->GetBlockCol();

				if(im_Attribs.im_LocalPos.x > blockCol - 1.0f && im_Attribs.im_LocalPos.x < blockCol + 1.0f
					&& im_Attribs.im_LocalPos.y > blockRow - 1.0f && im_Attribs.im_LocalPos.y < blockRow + 1.0f){
					val = -5;
				}
				break;
			}
			case EventID::EventAttacking: {
				const EventAttacking* const eventAttacking = dynamic_cast<const EventAttacking*>(myEvent);
				assert(eventAttacking && "Val of eventAttacking is nullptr!");

				const Vector3& eventAttackingLocalPos = eventAttacking->GetLocalPos();
				if(im_Attribs.im_LocalPos.x > eventAttackingLocalPos.x - 1.0f && im_Attribs.im_LocalPos.x < eventAttackingLocalPos.x + 1.0f
					&& im_Attribs.im_LocalPos.y > eventAttackingLocalPos.y - 1.0f && im_Attribs.im_LocalPos.y < eventAttackingLocalPos.y + 1.0f){
					val = 1;
					if(im_Attribs.im_CurrState->GetID() != StateID::StateBoyImmune){
						im_Attribs.im_CurrHealth -= eventAttacking->GetDmg();
					}
				}

				break;
			}
			case EventID::EventGridHeightShrinking: {
				const EventGridHeightShrinking* const eventGridHeightShrinking = dynamic_cast<const EventGridHeightShrinking*>(myEvent);
				assert(eventGridHeightShrinking && "Val of eventGridHeightShrinking is nullptr!");
				const int gridRows = eventGridHeightShrinking->GetGridRows();

				if(im_Attribs.im_LocalPos.y >= (float)gridRows - 1.0f){
					if(!gridRows){
						val = -5;
						break;
					}
					im_Attribs.im_LocalPos.y = float(gridRows - 1);
				}
				if(im_Attribs.im_GridTargetLocalPos.y >= (float)gridRows - 1.0f){
					im_Attribs.im_GridTargetLocalPos.y = float(gridRows - 1);
				}

				break;
			}
			case EventID::EventGridWidthShrinking: {
				const EventGridWidthShrinking* const eventGridWidthShrinking = dynamic_cast<const EventGridWidthShrinking*>(myEvent);
				assert(eventGridWidthShrinking && "Val of eventGridWidthShrinking is nullptr!");
				const int gridCols = eventGridWidthShrinking->GetGridCols();

				if(im_Attribs.im_LocalPos.x >= (float)gridCols - 1.0f){
					if(!gridCols){
						val = -5;
						break;
					}
					im_Attribs.im_LocalPos.x = float(gridCols - 1);
				}
				if(im_Attribs.im_GridTargetLocalPos.x >= (float)gridCols - 1.0f){
					im_Attribs.im_GridTargetLocalPos.x = float(gridCols - 1);
				}
				break;
			}
		}

		if(destroyEvent && myEvent){
			delete myEvent;
			myEvent = nullptr;
		}
		return val;
	}
}