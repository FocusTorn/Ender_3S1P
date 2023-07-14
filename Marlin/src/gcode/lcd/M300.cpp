/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if HAS_SOUND

#include "../gcode.h"

#include "../../lcd/marlinui.h" // i2c-based BUZZ
#include "../../libs/buzzer.h"  // Buzzer, if possible

#if ENABLED(CREALITY_TOUCHSCREEN)
  #include "../../lcd/e3v2/creality/lcd_rts.h"
#endif

/**
 * M300: Play beep sound S<frequency Hz> P<duration ms>
 */
void GcodeSuite::M300() {
  uint16_t duration = parser.ushortval('P', 1000);

  // Limits the tone duration to 0-5 seconds.
  NOMORE(duration, 5000U);

  #if ENABLED(CREALITY_TOUCHSCREEN)
    rtscheck.RTS_SndData(StartSoundSet, SoundAddr);
  #else
    uint16_t const frequency = parser.ushortval('S', 260);
    BUZZ(duration, frequency);
  #endif
}

#endif // HAS_SOUND