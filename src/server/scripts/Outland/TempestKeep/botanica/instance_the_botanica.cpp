/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "the_botanica.h"

class instance_the_botanica : public InstanceMapScript
{
    public:
        instance_the_botanica() : InstanceMapScript("instance_the_botanica", 553) { }

        struct instance_the_botanica_InstanceMapScript : public InstanceScript
        {
            instance_the_botanica_InstanceMapScript(Map* map) : InstanceScript(map)
            {
                CommanderSarannisGUID = ObjectGuid::Empty;
                HighBotanistFreywinnGUID = ObjectGuid::Empty;
                ThorngrinTheTenderGUID = ObjectGuid::Empty;
                LajGUID = ObjectGuid::Empty;
                WarpSplinterGUID = ObjectGuid::Empty;
            }

            void OnCreatureCreate(Creature* creature) override
            {
                switch (creature->GetEntry())
                {
                    case NPC_COMMANDER_SARANNIS:
                        CommanderSarannisGUID = creature->GetGUID();
                        break;
                    case NPC_HIGH_BOTANIST_FREYWINN:
                        HighBotanistFreywinnGUID = creature->GetGUID();
                        break;
                    case NPC_THORNGRIN_THE_TENDER:
                        ThorngrinTheTenderGUID = creature->GetGUID();
                        break;
                    case NPC_LAJ:
                        LajGUID = creature->GetGUID();
                        break;
                    case NPC_WARP_SPLINTER:
                        WarpSplinterGUID = creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            ObjectGuid GetGuidData(uint32 type) const override
            {
                switch (type)
                {
                    case DATA_COMMANDER_SARANNIS:
                        return CommanderSarannisGUID;
                    case DATA_HIGH_BOTANIST_FREYWINN:
                        return HighBotanistFreywinnGUID;
                    case DATA_THORNGRIN_THE_TENDER:
                        return ThorngrinTheTenderGUID;
                    case DATA_LAJ:
                        return LajGUID;
                    case DATA_WARP_SPLINTER:
                        return WarpSplinterGUID;
                    default:
                        break;
                }

                return ObjectGuid::Empty;
            }

            bool SetBossState(uint32 type, EncounterState state) override
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                switch (type)
                {
                    case DATA_COMMANDER_SARANNIS:
                    case DATA_HIGH_BOTANIST_FREYWINN:
                    case DATA_THORNGRIN_THE_TENDER:
                    case DATA_LAJ:
                    case DATA_WARP_SPLINTER:
                        break;
                    default:
                        break;
                }

                return true;
            }

            std::string GetSaveData() override
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "B O " << GetBossSaveData();

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(char const* str) override
            {
                if (!str)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(str);

                char dataHead1, dataHead2;

                std::istringstream loadStream(str);
                loadStream >> dataHead1 >> dataHead2;

                if (dataHead1 == 'B' && dataHead2 == 'O')
                {
                    for (uint8 i = 0; i < EncounterCount; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;

                        SetBossState(i, EncounterState(tmpState));
                    }
                }
                else
                    OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

        protected:
            ObjectGuid CommanderSarannisGUID;
            ObjectGuid HighBotanistFreywinnGUID;
            ObjectGuid ThorngrinTheTenderGUID;
            ObjectGuid LajGUID;
            ObjectGuid WarpSplinterGUID;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const override
        {
            return new instance_the_botanica_InstanceMapScript(map);
        }
};

void AddSC_instance_the_botanica()
{
    new instance_the_botanica;
}
