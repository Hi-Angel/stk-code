//  $Id$
//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2006 Joerg Henrichs
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "stk_config.hpp"
#include "file_manager.hpp"
#include "audio/music_information.hpp"

STKConfig* stk_config=0;
float STKConfig::UNDEFINED = -99.9f;

//-----------------------------------------------------------------------------
void STKConfig::load(const std::string filename)
{

    // Use the kart properties loader to read in the default kart
    // values, but don't try to load any models or materials       */
    KartProperties::load(filename, "config",
                        /*dont_load_models   */ true,
                        /*dont_load_materials*/ true  );

    // Check that all necessary values are indeed set 
    // -----------------------------------------------

#define CHECK_NEG(  a,strA) if(a<=UNDEFINED) {                         \
        fprintf(stderr,"Missing default value for '%s' in '%s'.\n",    \
                strA,filename.c_str());exit(-1);                       \
    }
    if(m_gear_switch_ratio.size()==0)
    {
        fprintf(stderr,"Missing default value for 'gear-switch-ratio' in '%s'.\n",
                filename.c_str());
        exit(-1);
    }
    if(m_gear_power_increase.size()==0)
    {
        fprintf(stderr,"Missing default value for 'gear-power-increase' in '%s'.\n",
                filename.c_str());
        exit(-1);
    }
    if(m_gear_switch_ratio.size()!=m_gear_power_increase.size())    {
        fprintf(stderr,"Number of entries for 'gear-switch-ratio' and 'gear-power-increase");
        fprintf(stderr,"in '%s' must be equal.\n", filename.c_str());
        exit(-1);
    }

    if(m_scores.size()==0 || (int)m_scores.size()!=m_max_karts)
    {
        fprintf(stderr,"Not or not enough scores defined in stk_config");
        exit(-1);
    }
    if(m_leader_intervals.size()==0)
    {
        fprintf(stderr,"No follow leader interval(s) defined in stk_config");
        exit(-1);
    }
    if(m_menu_background.size()==0)
    {
        fprintf(stderr,"No menu background defined in stk_config");
        exit(-1);
    }
    CHECK_NEG(m_max_karts,               "max-karts"                    );
    CHECK_NEG(m_grid_order,              "grid-order"                   );

    CHECK_NEG(m_mass,                    "mass"                         );
    CHECK_NEG(m_wheel_base,              "wheel-base"                   );
    CHECK_NEG(m_engine_power,            "engine-power"                 );
    CHECK_NEG(m_min_speed_turn,          "min-speed-angle"              );
    CHECK_NEG(m_angle_at_min,            "min-speed-angle"              );
    CHECK_NEG(m_max_speed_turn,          "max-speed-angle"              );
    CHECK_NEG(m_angle_at_max,            "max-speed-angle"              );
    CHECK_NEG(m_brake_factor,            "brake-factor"                 );

    CHECK_NEG(m_wheelie_max_speed_ratio, "wheelie-max-speed-ratio"      );
    CHECK_NEG(m_wheelie_max_pitch,       "wheelie-max-pitch"            );
    CHECK_NEG(m_wheelie_pitch_rate,      "wheelie-pitch-rate"           );
    CHECK_NEG(m_wheelie_restore_rate,    "wheelie-restore-rate"         );
    CHECK_NEG(m_wheelie_speed_boost,     "wheelie-speed-boost"          );
    CHECK_NEG(m_wheelie_power_boost,     "wheelie-power-boost"          );

    CHECK_NEG(m_parachute_friction,        "parachute-friction"         );
    CHECK_NEG(m_parachute_done_fraction,   "parachute-done-fraction"    );
    CHECK_NEG(m_parachute_time,            "parachute-time"             );
    CHECK_NEG(m_parachute_time_other,      "parachute-time-other"       );

    CHECK_NEG(m_time_full_steer,           "time-full-steer"            );

    //bullet physics data
    CHECK_NEG(m_suspension_stiffness,      "suspension-stiffness"       );
    CHECK_NEG(m_wheel_damping_relaxation,  "wheel-damping-relaxation"   );
    CHECK_NEG(m_wheel_damping_compression, "wheel-damping-compression"  );
    CHECK_NEG(m_friction_slip,             "friction-slip"              );
    CHECK_NEG(m_roll_influence,            "roll-influence"             );
    CHECK_NEG(m_wheel_radius,              "wheel-radius"               );
    CHECK_NEG(m_chassis_linear_damping,    "chassis-linear-damping"     );
    CHECK_NEG(m_chassis_angular_damping,   "chassis-angular-damping"    );
    CHECK_NEG(m_maximum_speed,             "maximum-speed"              );
    CHECK_NEG(m_max_speed_reverse_ratio,   "max-speed-reverse-ratio"    );
    CHECK_NEG(m_gravity_center_shift[0],   "gravity-center-shift"       );
    CHECK_NEG(m_gravity_center_shift[1],   "gravity-center-shift"       );
    CHECK_NEG(m_gravity_center_shift[2],   "gravity-center-shift"       );
    CHECK_NEG(m_bomb_time,                 "bomb-time"                  );
    CHECK_NEG(m_bomb_time_increase,        "bomb-time-increase"         );
    CHECK_NEG(m_anvil_time,                "anvil-time"                 );
    CHECK_NEG(m_anvil_weight,              "anvil-weight"               );
    CHECK_NEG(m_zipper_time,               "zipper-time"                );
    CHECK_NEG(m_zipper_force,              "zipper-force"               );
    CHECK_NEG(m_zipper_speed_gain,         "zipper-speed-gain"          );
    CHECK_NEG(m_shortcut_length,           "shortcut-length"  );
    CHECK_NEG(m_suspension_rest,           "suspension-rest"            );
    CHECK_NEG(m_suspension_travel_cm,      "suspension-travel-cm"       );
    CHECK_NEG(m_jump_velocity,             "jump-velocity"              );
    CHECK_NEG(m_explosion_impulse,         "explosion-impulse"          );
    CHECK_NEG(m_explosion_impulse_objects, "explosion-impulse-objects"  );
    CHECK_NEG(m_upright_tolerance,         "upright-tolerance"          );
    CHECK_NEG(m_upright_max_force,         "upright-max-force"          );
    CHECK_NEG(m_camera_max_accel,          "camera-max-accel"           );
    CHECK_NEG(m_camera_max_brake,          "camera-max-brake"           );
    CHECK_NEG(m_camera_distance,           "camera-distance"            );
    CHECK_NEG(m_max_history,               "max-history"                );
    CHECK_NEG(m_delay_finish_time,         "delay-finish-time"          );
    CHECK_NEG(m_music_credit_time,         "music-credit-time"          );

}   // load

// -----------------------------------------------------------------------------
/** Init all values with invalid defaults, which are tested later. This
 * guarantees that all parameters will indeed be initialised, and helps
 * finding typos.
 */
void STKConfig::init_defaults()
{
    m_wheel_base   = m_mass = 
    m_min_speed_turn = m_angle_at_min = m_max_speed_turn = m_angle_at_max = 
    m_anvil_weight    = m_parachute_friction =
    m_parachute_time = m_parachute_done_fraction = m_parachute_time_other = 
    m_engine_power = m_brake_factor =
    m_anvil_speed_factor = m_time_full_steer = m_wheelie_max_pitch =
    m_wheelie_max_speed_ratio = m_wheelie_pitch_rate = 
    m_wheelie_restore_rate = m_wheelie_speed_boost = 
    m_bomb_time = m_bomb_time_increase= m_anvil_time = 
    m_zipper_time = m_zipper_force = m_zipper_speed_gain = 
    m_shortcut_length = m_music_credit_time = m_delay_finish_time =
    //bullet physics data
    m_suspension_stiffness = m_wheel_damping_relaxation = 
    m_wheel_damping_compression = m_friction_slip = m_roll_influence = 
    m_wheel_radius = m_wheelie_power_boost = 
    m_chassis_linear_damping = m_chassis_angular_damping = 
    m_maximum_speed = m_suspension_rest = 
    m_max_speed_reverse_ratio = m_explosion_impulse = m_jump_velocity = 
    m_explosion_impulse_objects = m_upright_tolerance = m_upright_max_force =
	m_suspension_travel_cm =
    // Camera
    m_camera_max_accel = m_camera_max_brake = m_camera_distance = UNDEFINED;
    m_gravity_center_shift   = Vec3(UNDEFINED);
    m_front_wheel_connection = Vec3(UNDEFINED);
    m_rear_wheel_connection  = Vec3(UNDEFINED);
    m_max_karts              = -100;
    m_grid_order             = -100;
    m_max_history            = -100;
    m_title_music            = NULL;
    m_scores.clear();
    m_leader_intervals.clear();
}   // init_defaults

//-----------------------------------------------------------------------------
void STKConfig::getAllData(const lisp::Lisp* lisp)
{

    // Get the values which are not part of the default KartProperties
    // ---------------------------------------------------------------
    lisp->get("anvil-weight",                 m_anvil_weight             );
    lisp->get("shortcut-length",              m_shortcut_length        );
    lisp->get("anvil-speed-factor",           m_anvil_speed_factor       );
    lisp->get("parachute-friction",           m_parachute_friction       );
    lisp->get("parachute-time",               m_parachute_time           );
    lisp->get("parachute-time-other",         m_parachute_time_other     );
    lisp->get("parachute-done-fraction",      m_parachute_done_fraction  );
    lisp->get("bomb-time",                    m_bomb_time                );
    lisp->get("bomb-time-increase",           m_bomb_time_increase       );
    lisp->getVector("leader-intervals",       m_leader_intervals         );
    lisp->get("anvil-time",                   m_anvil_time               );
    lisp->get("zipper-time",                  m_zipper_time              );
    lisp->get("zipper-force",                 m_zipper_force             );
    lisp->get("zipper-speed-gain",            m_zipper_speed_gain        );
    lisp->get("explosion-impulse",            m_explosion_impulse        );
    lisp->get("explosion-impulse-objects",    m_explosion_impulse_objects);
    lisp->get("max-karts",                    m_max_karts                );
    lisp->get("grid-order",                   m_grid_order               );
    lisp->getVector("scores",                 m_scores                   );
    lisp->get("max-history",                  m_max_history              );
    lisp->get("delay-finish-time",            m_delay_finish_time        );
    lisp->get("music-credit-time",            m_music_credit_time        );
    lisp->get("menu-background",              m_menu_background          );
    std::string title_music;
    lisp->get("title-music",                  title_music                );
    m_title_music = new MusicInformation(file_manager->getMusicFile(title_music));

    // Get the default KartProperties
    // ------------------------------
    KartProperties::getAllData(lisp->getLisp("kart-defaults"));
}   // getAllData
