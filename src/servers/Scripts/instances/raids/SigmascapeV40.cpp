#include <ScriptObject.h>
#include <Zone/InstanceContent.h>

using namespace Core;

class SigmascapeV40 :
  public Sapphire::ScriptAPI::InstanceContentScript
{
public:
  SigmascapeV40() :
    Sapphire::ScriptAPI::InstanceContentScript( 30062 )
  {
  }

  void onInit( InstanceContentPtr instance ) override
  {

  }

  void onUpdate( InstanceContentPtr instance, uint32_t currTime ) override
  {

  }

  void onEnterTerritory( InstanceContentPtr instance, Entity::Player& player, uint32_t eventId, uint16_t param1,
                         uint16_t param2 ) override
  {

  }

};