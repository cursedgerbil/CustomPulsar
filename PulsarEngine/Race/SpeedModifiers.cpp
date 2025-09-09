#include <kamek.hpp>
#include <MarioKartWii/Race/RaceInfo/RaceInfo.hpp>
#include <MarioKartWii/3D/Model/ModelDirector.hpp>
#include <MarioKartWii/Kart/KartValues.hpp>
#include <MarioKartWii/Kart/KartMovement.hpp>
#include <MarioKartWii/Item/Obj/ObjProperties.hpp>
#include <MarioKartWii/RKNet/RKNetController.hpp>
#include <MarioKartWii/File/StatsParam.hpp>
#include <Race/200ccParams.hpp>
#include <PulsarSystem.hpp>
#include <RetroRewind.hpp>
#include <MarioKartWii/File/RKG.hpp>

namespace Pulsar {

  Kart::Stats* TransmissionChanges(KartId kartId, CharacterId characterId, KartType kartType) {
      Kart::Stats* stats = Kart::ComputeStats(kartId, characterId);
    
      bool insideBike = false;
      bool insideKart = false;
      bool vanilla = true;
      bool isGhost = false;
      bool isLocal = false;
      const RacedataScenario& scenario = Racedata::sInstance->racesScenario;
      u32 transmission = static_cast<Pulsar::Transmission>(Pulsar::Settings::Mgr::Get().GetUserSettingValue(
        static_cast<Pulsar::Settings::UserType>(Pulsar::Settings::SETTINGSTYPE_GAMEPLAY),
        Pulsar::SETTINGGAME_TRANSMISSION));

      //Check if local or ghost player
      for (int playerId = 0; playerId < scenario.playerCount; ++playerId) {
          if (scenario.players[playerId].kartId == kartId &&
              scenario.players[playerId].characterId == characterId) {
              if (scenario.players[playerId].playerType == PLAYER_REAL_LOCAL) {
                  isLocalPlayer = true;
              } else if (scenario.players[playerId].playerType == PLAYER_GHOST) {
                  isGhostPlayer = true;
              }
          }
      }

     //Check which ghost is the player
     int ghostPlayerId = -1;
     if (isGhostPlayer) {
        for (int ghostId = 0; ghostId < scenario.playerCount; ++ghostId) {
            if (scenario.players[ghostId].kartId == kartId &&
                scenario.players[ghostId].characterId == characterId &&
                scenario.players[ghostId].playerType == PLAYER_GHOST) {
                ghostPlayerId = ghostId;
                break;
            }
        }
    }
    

      //Implement transmission switch
      if (isLocalPlayer || ghostPlayerId >= 0) {
        if (transmission == Pulsar::TRANSMISSION_INSIDE) {
          insideBike = true;
          insideKart = true;
          vanilla = false;
        }
        else if (transmission == Pulsar::TRANSMISSION_OUTSIDE) {
          insideBike = false;
          insideKart = false;
          vanilla = false;
        }
        else if (transmission == Pulsar::TRANSMISSION_BIKES) {
          insideBike = true;
          insideKart = false;
          vanilla = false;
        }

        if (vanilla == false) {
          
          if (stats->type == INSIDE_BIKE || stats->type == OUTSIDE_BIKE) {
            if (insideBike = true) {
              stats->type = INSIDE_BIKE;
              stats->targetAngle = 0.0f;
            }
            else {
              stats->type = OUTSIDE_BIKE;
              stats->targetAngle = 45.0f;
            }
          }
          
          if (stats->type == KART) {
            if (insideKart = true) {
              stats->type = INSIDE_BIKE;
              stats->mt += 100.0f;
            }
            else {
              stats->type = KART;
            }
          }
        }
      }

    return stats;
  }

kmCall(0x8058f670, TransmissionChanges);

}
