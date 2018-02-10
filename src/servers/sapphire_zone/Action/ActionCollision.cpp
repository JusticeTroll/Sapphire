#include <common/Util/Util.h>
#include <common/Util/UtilMath.h>

#include "ActionCollision.h"
#include "Actor/Actor.h"
#include "Actor/Player.h"

#include <cmath>
#include <boost/make_shared.hpp>

using namespace Core::Entity;
using namespace Core::Common;

// todo: add AoE actor limits (16, 32)

bool ActionCollision::isActorApplicable( Actor& actor, TargetFilter targetFilter )
{
   bool actorApplicable = false;
   switch( targetFilter )
   {
   case TargetFilter::All:
   {
      actorApplicable = true;
      break;
   }
   case TargetFilter::Players:
   {
      actorApplicable = actor.isPlayer();
      break;
   }
   case TargetFilter::Allies:
   {
      // todo: implement ally NPCs
      actorApplicable = !actor.isBattleNpc();
      break;
   }
   case TargetFilter::Party:
   {
      // todo: implement party
      actorApplicable = actor.isPlayer();
      break;
   }
   case TargetFilter::Enemies:
   {
      actorApplicable = actor.isBattleNpc();
      break;
   }
   }

   return ( actorApplicable && actor.isAlive() );
}

std::set< Core::Entity::ActorPtr > ActionCollision::getActorsHitFromAction( FFXIVARR_POSITION3 aoePosition,
                                                                            std::set< ActorPtr > actorsInRange,
                                                                            boost::shared_ptr< Core::Data::Action > actionInfo,
                                                                            TargetFilter targetFilter )
{
   std::set< ActorPtr > actorsCollided;

   switch( static_cast< ActionCollisionType >( actionInfo->castType ) )
   {
   case ActionCollisionType::None:
   case ActionCollisionType::SingleTarget:
   {
      // This is actually needed. There is "splash damage" in actions marked as single target.
      // Notice how we're using aoe_width. How collision works for SingleTarget is unknown as of now.
      for( auto pActor : actorsInRange )
      {
         // Make sure actor exists. If it doesn't we done goofed.
         assert( pActor );

         // Don't bother wasting on collision if actor doesn't apply for it
         if ( !isActorApplicable( *pActor, targetFilter ) )
            continue;

         // Test our collision from actor with the area generated by the action from the AoE data
         if ( radiusCollision( pActor->getPos(), aoePosition, actionInfo->effectRange ) )
         {
            // Add it to the actors collided with the area
            actorsCollided.insert( pActor );
         }
      }
      break;
   }
   case ActionCollisionType::Circle:
   {
      for( auto pActor : actorsInRange )
      {
         assert( pActor );

         if ( !isActorApplicable( *pActor, targetFilter ) )
            continue;

         if ( radiusCollision( pActor->getPos(), aoePosition, actionInfo->effectRange ) )
            actorsCollided.insert( pActor );
      }
      break;
   }
   case ActionCollisionType::Box:
   {
      for( auto pActor : actorsInRange )
      {
         assert( pActor );

         if ( !isActorApplicable( *pActor, targetFilter ) )
            continue;

         if ( boxCollision( pActor->getPos(), aoePosition, actionInfo->xAxisModifier, actionInfo->effectRange ) )
         {
            // todo: does this actually work?

            actorsCollided.insert( pActor );
         }
      }
      break;
   }
   default:
   {
      break;
   }
   }

   return actorsCollided;
}

bool ActionCollision::radiusCollision( FFXIVARR_POSITION3 actorPosition, FFXIVARR_POSITION3 aoePosition, uint16_t radius )
{
   return Core::Math::Util::distance( actorPosition.x, actorPosition.y, actorPosition.z,
      aoePosition.x, aoePosition.y, aoePosition.z ) <= radius;
}

bool ActionCollision::boxCollision( FFXIVARR_POSITION3 actorPosition, FFXIVARR_POSITION3 aoePosition, uint16_t width, uint16_t height )
{
   return actorPosition.x < aoePosition.x + width &&
      actorPosition.x > aoePosition.x &&
      actorPosition.y < aoePosition.y + height &&
      actorPosition.y > aoePosition.y;
}
