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

#if HAS_BED_PROBE

#include "../gcode.h"
#include "../../feature/bedlevel/bedlevel.h"
#include "../../module/probe.h"

#if ENABLED(CREALITY_TOUCHSCREEN)
  #include "../../lcd/e3v2/creality/lcd_rts.h"
#endif

/**
 * M851: Set the nozzle-to-probe offsets in current units
 */
void GcodeSuite::M851() {
  // No parameters? Show current state.
  if (!parser.seen("XYZ")) return M851_report();

  // Start with current offsets and modify
  xyz_pos_t offs = probe.offset;

  // Assume no errors
  bool ok = true;

  if (parser.seenval('X')) {
    const float x = parser.value_float();
    #if HAS_PROBE_XY_OFFSET
      if (WITHIN(x, -(X_BED_SIZE), X_BED_SIZE))
        offs.x = x;
      else {
        SERIAL_ECHOLNPGM("?X out of range (-", X_BED_SIZE, " to ", X_BED_SIZE, ")");
        ok = false;
      }
    #else
      if (x) SERIAL_ECHOLNPGM("?X must be 0 (NOZZLE_AS_PROBE)."); // ...but let 'ok' stay true
    #endif
  }

  if (parser.seenval('Y')) {
    const float y = parser.value_float();
    #if HAS_PROBE_XY_OFFSET
      if (WITHIN(y, -(Y_BED_SIZE), Y_BED_SIZE))
        offs.y = y;
      else {
        SERIAL_ECHOLNPGM("?Y out of range (-", Y_BED_SIZE, " to ", Y_BED_SIZE, ")");
        ok = false;
      }
    #else
      if (y) SERIAL_ECHOLNPGM("?Y must be 0 (NOZZLE_AS_PROBE)."); // ...but let 'ok' stay true
    #endif
  }

  if (parser.seenval('Z')) {
    const float z = parser.value_float();
    if (WITHIN(z, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
      #if ENABLED(CREALITY_TOUCHSCREEN)
        zprobe_zoffset = z;
        rtscheck.RTS_SndData(zprobe_zoffset * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
      #endif

      offs.z = z;
    } else {
      SERIAL_ECHOLNPGM("?Z out of range (", Z_PROBE_OFFSET_RANGE_MIN, " to ", Z_PROBE_OFFSET_RANGE_MAX, ")");
      ok = false;
    }
  }

  // Save the new offsets
  if (ok) probe.offset = offs;
}

void GcodeSuite::M851_report(const bool forReplay/*=true*/) {
  report_heading_etc(forReplay, F(STR_Z_PROBE_OFFSET));
  SERIAL_ECHOPGM_P(
    #if HAS_PROBE_XY_OFFSET
      PSTR("  M851 Probe Offset X"), LINEAR_UNIT(probe.offset_xy.x),
              SP_Y_STR, LINEAR_UNIT(probe.offset_xy.y),
              SP_Z_STR
    #else
      PSTR("  M851 Probe Offset X0 Y0 Z")
    #endif
    , LINEAR_UNIT(probe.offset.z)
    , PSTR(" ;")
  );
  say_units();

  #if ENABLED(CREALITY_TOUCHSCREEN)
    zprobe_zoffset = probe.offset.z;
    rtscheck.RTS_SndData(probe.offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
  #endif

}

#endif // HAS_BED_PROBE
