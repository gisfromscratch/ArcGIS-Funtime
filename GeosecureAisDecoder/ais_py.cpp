// TODO(schwehr): check for reference counting leaks
// TODO(schwehr): better error handling for all messages

#include <Python.h>

#include "ais.h"
#include "ais8_001_22.h"

PyObject *ais_py_exception;
const string exception_name("ais.decode.Error");

#if 0
// DO NOT do this
void TupleSafeSetItem(PyObject *tuple, const long position, const long val) {
  assert(tuple);
  PyObject *val_obj = PyLong_FromLong(val);
  assert(val_obj);
  PyTuple_SetItem(tuple, position, val_obj);
  Py_DECREF(val_obj);
}
#endif

void
DictSafeSetItem(PyObject *dict, const string &key, const long val) {
  PyObject *key_obj = PyUnicode_FromString(key.c_str());
  PyObject *val_obj = PyLong_FromLong(val);
  assert(key_obj);
  assert(val_obj);
  PyDict_SetItem(dict, key_obj, val_obj);
  Py_DECREF(key_obj);
  Py_DECREF(val_obj);
}

void
DictSafeSetItem(PyObject *dict, const string &key, const int val) {
  PyObject *key_obj = PyUnicode_FromString(key.c_str());
  PyObject *val_obj = PyLong_FromLong(val);
  assert(key_obj);
  assert(val_obj);
  PyDict_SetItem(dict, key_obj, val_obj);
  Py_DECREF(key_obj);
  Py_DECREF(val_obj);
}

void
DictSafeSetItem(PyObject *dict, const string &key, const string &val) {
  PyObject *key_obj = PyUnicode_FromString(key.c_str());
  PyObject *val_obj = PyUnicode_FromString(val.c_str());
  assert(key_obj);
  assert(val_obj);
  PyDict_SetItem(dict, key_obj, val_obj);
  Py_DECREF(key_obj);
  Py_DECREF(val_obj);
}


void
DictSafeSetItem(PyObject *dict, const string &key, const char *val) {
  PyObject *key_obj = PyUnicode_FromString(key.c_str());
  PyObject *val_obj = PyUnicode_FromString(val);
  assert(key_obj);
  assert(val_obj);
  PyDict_SetItem(dict, key_obj, val_obj);
  Py_DECREF(key_obj);
  Py_DECREF(val_obj);
}


#if 0
void
DictSafeSetItem(PyObject *dict, const string &key, const bool val) {
  PyObject *key_obj = PyUnicode_FromString(key.c_str());
  PyObject *val_obj = PyBool_FromLong(val);
  assert(key_obj);
  assert(val_obj);
  PyDict_SetItem(dict, key_obj, val_obj);
  Py_DECREF(key_obj);
  Py_DECREF(val_obj);
}
#else
void
DictSafeSetItem(PyObject *dict, const string &key, const bool val) {
  PyObject *key_obj = PyUnicode_FromString(key.c_str());
  assert(key_obj);
  if (val) {
    PyDict_SetItem(dict, key_obj, Py_True);
  } else {
    PyDict_SetItem(dict, key_obj, Py_False);
  }
  Py_DECREF(key_obj);
}
#endif

// TODO(schwehr): float -> double?
void
DictSafeSetItem(PyObject *dict, const string &key, const float val) {
  PyObject *key_obj = PyUnicode_FromString(key.c_str());
  PyObject *val_obj = PyFloat_FromDouble(val);
  assert(key_obj);
  assert(val_obj);
  PyDict_SetItem(dict, key_obj, val_obj);
  Py_DECREF(key_obj);
  Py_DECREF(val_obj);
}

void
DictSafeSetItem(PyObject *dict, const string &key, PyObject *val_obj) {
  // When we need to add dictionaries and such to a dictionary
  assert(dict);
  assert(val_obj);
  PyObject *key_obj = PyUnicode_FromString(key.c_str());
  assert(key_obj);
  PyDict_SetItem(dict, key_obj, val_obj);
  Py_DECREF(key_obj);
}


extern "C" {

PyObject *
ais1_2_3_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);

  Ais1_2_3 msg(nmea_payload, pad);
  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais1_2_3: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", msg.message_id);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);

  DictSafeSetItem(dict, "nav_status", msg.nav_status);
  DictSafeSetItem(dict, "rot_over_range", msg.rot_over_range);
  DictSafeSetItem(dict, "rot", msg.rot);
  DictSafeSetItem(dict, "sog", msg.sog);
  DictSafeSetItem(dict, "position_accuracy", msg.position_accuracy);
  DictSafeSetItem(dict, "x", msg.x);
  DictSafeSetItem(dict, "y", msg.y);
  DictSafeSetItem(dict, "cog", msg.cog);
  DictSafeSetItem(dict, "true_heading", msg.true_heading);
  DictSafeSetItem(dict, "timestamp", msg.timestamp);
  DictSafeSetItem(dict, "special_manoeuvre", msg.special_manoeuvre);
  DictSafeSetItem(dict, "spare", msg.spare);
  DictSafeSetItem(dict, "raim", msg.raim);

  // COMM States
  DictSafeSetItem(dict, "sync_state", msg.sync_state);  // Both SOTDMA & ITDMA

  // SOTDMA
  if (msg.message_id == 1 || msg.message_id == 2) {
    DictSafeSetItem(dict, "slot_timeout", msg.slot_timeout);

    if (msg.received_stations_valid)
      DictSafeSetItem(dict, "received_stations", msg.received_stations);
    if (msg.slot_number_valid)
      DictSafeSetItem(dict, "slot_number", msg.slot_number);
    if (msg.utc_valid) {
      DictSafeSetItem(dict, "utc_hour", msg.utc_hour);
      DictSafeSetItem(dict, "utc_min", msg.utc_min);
      DictSafeSetItem(dict, "utc_spare", msg.utc_spare);
    }

    if (msg.slot_offset_valid)
      DictSafeSetItem(dict, "slot_offset", msg.slot_offset);
  }

  // ITDMA
  if (msg.slot_increment_valid) {
    DictSafeSetItem(dict, "slot_increment", msg.slot_increment);
    DictSafeSetItem(dict, "slots_to_allocate", msg.slots_to_allocate);
    DictSafeSetItem(dict, "keep_flag", msg.keep_flag);
  }

  return dict;
}


PyObject *
ais4_11_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);

  Ais4_11 msg(nmea_payload, pad);
  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais4_11: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", msg.message_id);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);

  DictSafeSetItem(dict, "year", msg.year);
  DictSafeSetItem(dict, "month", msg.month);
  DictSafeSetItem(dict, "day", msg.day);
  DictSafeSetItem(dict, "hour", msg.hour);
  DictSafeSetItem(dict, "minute", msg.minute);
  DictSafeSetItem(dict, "second", msg.second);

  DictSafeSetItem(dict, "position_accuracy", msg.position_accuracy);
  DictSafeSetItem(dict, "x", msg.x);
  DictSafeSetItem(dict, "y", msg.y);

  DictSafeSetItem(dict, "fix_type", msg.fix_type);
  DictSafeSetItem(dict, "transmission_ctl", msg.transmission_ctl);
  DictSafeSetItem(dict, "spare", msg.spare);
  DictSafeSetItem(dict, "raim", msg.raim);

  // SOTDMA
  DictSafeSetItem(dict, "sync_state", msg.sync_state);
  DictSafeSetItem(dict, "slot_timeout", msg.slot_timeout);

  if (msg.received_stations_valid)
    DictSafeSetItem(dict, "received_stations", msg.received_stations);
  if (msg.slot_number_valid)
    DictSafeSetItem(dict, "slot_number", msg.slot_number);
  if (msg.utc_valid) {
    DictSafeSetItem(dict, "utc_hour", msg.utc_hour);
    DictSafeSetItem(dict, "utc_min", msg.utc_min);
    DictSafeSetItem(dict, "utc_spare", msg.utc_spare);
  }

  if (msg.slot_offset_valid)
    DictSafeSetItem(dict, "slot_offset", msg.slot_offset);

  return dict;
}


PyObject *
ais5_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);
  Ais5 msg(nmea_payload, pad);
  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais5: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();

  DictSafeSetItem(dict, "id", msg.message_id);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);

  DictSafeSetItem(dict, "ais_version", msg.ais_version);
  DictSafeSetItem(dict, "imo_num", msg.imo_num);
  DictSafeSetItem(dict, "callsign", msg.callsign);
  DictSafeSetItem(dict, "name", msg.name);
  DictSafeSetItem(dict, "type_and_cargo", msg.type_and_cargo);
  DictSafeSetItem(dict, "dim_a", msg.dim_a);
  DictSafeSetItem(dict, "dim_b", msg.dim_b);
  DictSafeSetItem(dict, "dim_c", msg.dim_c);
  DictSafeSetItem(dict, "dim_d", msg.dim_d);
  DictSafeSetItem(dict, "fix_type", msg.fix_type);
  DictSafeSetItem(dict, "eta_month", msg.eta_month);
  DictSafeSetItem(dict, "eta_day", msg.eta_day);
  DictSafeSetItem(dict, "eta_hour", msg.eta_hour);
  DictSafeSetItem(dict, "eta_minute", msg.eta_minute);
  DictSafeSetItem(dict, "draught", msg.draught);
  DictSafeSetItem(dict, "destination", msg.destination);
  DictSafeSetItem(dict, "dte", msg.dte);
  DictSafeSetItem(dict, "spare", msg.spare);

  return dict;
}


void
ais6_1_0_append_pydict(const char *nmea_payload, PyObject *dict,
                       const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais6_1_0 msg(nmea_payload, pad);
  DictSafeSetItem(dict, "ack_required",  msg.ack_required);
  DictSafeSetItem(dict, "msg_seq",  msg.msg_seq);
  DictSafeSetItem(dict, "text", msg.text);
  DictSafeSetItem(dict, "spare2", msg.spare2);
}

void
ais6_1_1_append_pydict(const char *nmea_payload, PyObject *dict,
                       const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais6_1_1 msg(nmea_payload, pad);  // TODO(schwehr): check for error
  DictSafeSetItem(dict, "ack_dac", msg.ack_dac);
  DictSafeSetItem(dict, "msg_seq", msg.msg_seq);
  DictSafeSetItem(dict, "spare2", msg.spare2);
}

void
ais6_1_2_append_pydict(const char *nmea_payload, PyObject *dict,
                       const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais6_1_2 msg(nmea_payload, pad);  // TODO(schwehr): check for error
  DictSafeSetItem(dict, "req_dac", msg.req_dac);
  DictSafeSetItem(dict, "req_fi", msg.req_fi);
}

void
ais6_1_3_append_pydict(const char *nmea_payload, PyObject *dict,
                       const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais6_1_3 msg(nmea_payload, pad);  // TODO(schwehr): check for error
  DictSafeSetItem(dict, "req_dac", msg.req_dac);
  DictSafeSetItem(dict, "spare2", msg.spare2);
}

void
ais6_1_4_append_pydict(const char *nmea_payload, PyObject *dict,
                       const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais6_1_4 msg(nmea_payload, pad);  // TODO(schwehr): check for error
  if (msg.had_error()) return;  // TODO(schwehr): do better

  DictSafeSetItem(dict, "ack_dac", msg.ack_dac);
  PyObject *cap_list = PyList_New(26);
  PyObject *res_list = PyList_New(26);
  for (size_t cap_num = 0; cap_num < 128/2; cap_num++) {
    // TODO(schwehr): memory leak?
    PyObject *cap = PyInt_FromLong(long(msg.capabilities[cap_num]));
    PyList_SetItem(cap_list, cap_num, cap);

    PyObject *res = PyInt_FromLong(long(msg.cap_reserved[cap_num]));
    PyList_SetItem(res_list, cap_num, res);
  }
  DictSafeSetItem(dict, "capabilities", cap_list);
  DictSafeSetItem(dict, "cap_reserved", res_list);
  DictSafeSetItem(dict, "spare2", msg.spare2);
}

void
ais6_1_40_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais6_1_40 msg(nmea_payload, pad);  // TODO(schwehr): check for error
  DictSafeSetItem(dict, "persons", msg.persons);
  DictSafeSetItem(dict, "spare2", msg.spare2);
}


bool
ais6_1_12_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  assert(nmea_payload);
  assert(pad < 6);
  Ais6_1_12 msg(nmea_payload, pad);
  if (msg.had_error()) {
    // TODO(schwehr): unify the error handling.
    return false;
  }

  DictSafeSetItem(dict, "last_port", msg.last_port);
  DictSafeSetItem(dict, "utc_month_dep", msg.utc_month_dep);  // actual
  DictSafeSetItem(dict, "utc_day_dep", msg.utc_day_dep);
  DictSafeSetItem(dict, "utc_hour_dep", msg.utc_hour_dep);
  DictSafeSetItem(dict, "utc_min_dep", msg.utc_min_dep);
  DictSafeSetItem(dict, "next_port", msg.next_port);
  DictSafeSetItem(dict, "utc_month_next", msg.utc_month_next);  // estimated
  DictSafeSetItem(dict, "utc_day_next", msg.utc_day_next);
  DictSafeSetItem(dict, "utc_hour_next", msg.utc_hour_next);
  DictSafeSetItem(dict, "utc_min_next", msg.utc_min_next);
  DictSafeSetItem(dict, "main_danger", msg.main_danger);
  DictSafeSetItem(dict, "imo_cat", msg.imo_cat);
  DictSafeSetItem(dict, "un", msg.un);
  DictSafeSetItem(dict, "value", msg.value);  // TODO(schwehr): units?
  DictSafeSetItem(dict, "value_unit", msg.value_unit);
  DictSafeSetItem(dict, "spare2", msg.spare2);
  return true;
}

// 6_1_13 does not exist

// IMO Circ 289 - Tidal Window
void
ais6_1_14_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  assert(nmea_payload);
  assert(pad < 6);
  Ais6_1_14 msg(nmea_payload, pad);  // TODO(schwehr): check for error
  if (msg.had_error()) {
    return;
  }

  DictSafeSetItem(dict, "utc_month", msg.utc_month);
  DictSafeSetItem(dict, "utc_day", msg.utc_day);

  if (!msg.windows.size())
    return;  // TODO(schwehr): is this an error?

  PyObject *window_list = PyList_New(msg.windows.size());
  for (size_t w_num = 0; w_num < msg.windows.size(); w_num++) {
    PyObject *window = PyDict_New();
    DictSafeSetItem(window, "y", msg.windows[w_num].x);
    DictSafeSetItem(window, "x", msg.windows[w_num].y);  // bits are lat, lon
    DictSafeSetItem(window, "utc_hour_from", msg.windows[w_num].utc_hour_from);
    DictSafeSetItem(window, "utc_min_from", msg.windows[w_num].utc_min_from);
    DictSafeSetItem(window, "utc_hour_to", msg.windows[w_num].utc_hour_to);
    DictSafeSetItem(window, "utc_min_to", msg.windows[w_num].utc_min_to);
    DictSafeSetItem(window, "cur_dir", msg.windows[w_num].cur_dir);
    DictSafeSetItem(window, "cur_speed", msg.windows[w_num].cur_speed);
    PyList_SetItem(window_list, w_num, window);
  }
  PyDict_SetItem(dict, PyUnicode_FromString("windows"), window_list);
}

// 6_1_15, 6_1_16, and 6_1_17 do not exist

// IMO Circ 289 - Clearance time to enter port
void
ais6_1_18_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  assert(nmea_payload);
  assert(pad < 6);
  Ais6_1_18 msg(nmea_payload, pad);  // TODO(schwehr): check for error

  DictSafeSetItem(dict, "link_id", msg.link_id);
  DictSafeSetItem(dict, "utc_month", msg.utc_month);
  DictSafeSetItem(dict, "utc_day", msg.utc_day);
  DictSafeSetItem(dict, "utc_hour", msg.utc_hour);
  DictSafeSetItem(dict, "utc_min", msg.utc_min);
  DictSafeSetItem(dict, "port_berth", msg.port_berth);
  DictSafeSetItem(dict, "dest", msg.dest);
  DictSafeSetItem(dict, "x", msg.x);
  DictSafeSetItem(dict, "y", msg.y);
  DictSafeSetItem(dict, "spare2_0", msg.spare2[0]);
  DictSafeSetItem(dict, "spare2_1", msg.spare2[1]);
}

// 6_1_19 does not exist

// IMO Circ 289 - Berthing data
void
ais6_1_20_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  assert(nmea_payload);
  assert(pad < 6);
  Ais6_1_20 msg(nmea_payload, pad);  // TODO(schwehr): check for error
  if (msg.had_error())
    return;

  DictSafeSetItem(dict, "link_id", msg.link_id);
  DictSafeSetItem(dict, "length", msg.length);
  DictSafeSetItem(dict, "depth", msg.depth);
  DictSafeSetItem(dict, "position", msg.position);
  DictSafeSetItem(dict, "utc_month", msg.utc_month);
  DictSafeSetItem(dict, "utc_day", msg.utc_day);
  DictSafeSetItem(dict, "utc_hour", msg.utc_hour);
  DictSafeSetItem(dict, "utc_min", msg.utc_min);
  if (msg.services_known) {
    PyObject *serv_list = PyList_New(26);
    for (size_t serv_num = 0; serv_num < 26; serv_num++) {
      PyObject *serv = PyInt_FromLong(long(msg.services[serv_num]));
      PyList_SetItem(serv_list, serv_num, serv);
    }
    DictSafeSetItem(dict, "services", serv_list);
  }
  DictSafeSetItem(dict, "name", msg.name);
  DictSafeSetItem(dict, "x", msg.x);
  DictSafeSetItem(dict, "y", msg.y);
}

// 6_1_21, 6_1_22, 6_1_23 and 6_1_24 Do not exist (yet?)

void
ais6_1_25_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais6_1_25 msg(nmea_payload, pad);  // TODO(schwehr): check for error
  if (msg.had_error()) return;

  DictSafeSetItem(dict, "amount_unit", msg.amount_unit);
  DictSafeSetItem(dict, "amount", msg.amount);

  if (0 == msg.cargos.size())
    return;  // TODO(schwehr): is this an error?

  PyObject *cargo_list = PyList_New(msg.cargos.size());
  for (size_t cargo_num = 0; cargo_num < msg.cargos.size(); cargo_num++) {
    PyObject *cargo = PyDict_New();
    if (msg.cargos[cargo_num].imdg_valid)
      DictSafeSetItem(cargo, "imdg", msg.cargos[cargo_num].imdg);
    if (msg.cargos[cargo_num].spare_valid)
      DictSafeSetItem(cargo, "spare", msg.cargos[cargo_num].spare);
    if (msg.cargos[cargo_num].un_valid)
      DictSafeSetItem(cargo, "un", msg.cargos[cargo_num].un);
    if (msg.cargos[cargo_num].bc_valid)
      DictSafeSetItem(cargo, "bc", msg.cargos[cargo_num].bc);
    if (msg.cargos[cargo_num].marpol_oil_valid)
      DictSafeSetItem(cargo, "marpol_oil", msg.cargos[cargo_num].marpol_oil);
    if (msg.cargos[cargo_num].marpol_cat_valid)
      DictSafeSetItem(cargo, "marpol_cat", msg.cargos[cargo_num].marpol_cat);
    PyList_SetItem(cargo_list, cargo_num, cargo);
  }
  PyDict_SetItem(dict, PyUnicode_FromString("cargos"), cargo_list);
}

// 6_1_26
// 6_1_27
// 6_1_28 - TODO(schwehr): Route Addressed - clone from 8_1_27
// 6_1_29
// 6_1_30 - TODO(schwehr): Text Addressed
// 6_1_31

void
ais6_1_32_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais6_1_32 msg(nmea_payload, pad);  // TODO(schwehr): check for error

  DictSafeSetItem(dict, "utc_month", msg.utc_month);
  DictSafeSetItem(dict, "utc_day", msg.utc_day);

  PyObject *window_list = PyList_New(msg.windows.size());
  for (size_t win_num = 0; win_num < msg.windows.size(); win_num++) {
    PyObject *win = PyDict_New();
    DictSafeSetItem(win, "x", msg.windows[win_num].x);
    DictSafeSetItem(win, "y", msg.windows[win_num].y);
    DictSafeSetItem(win, "from_utc_hour", msg.windows[win_num].from_utc_hour);
    DictSafeSetItem(win, "from_utc_min", msg.windows[win_num].from_utc_min);
    DictSafeSetItem(win, "to_utc_hour", msg.windows[win_num].to_utc_hour);
    DictSafeSetItem(win, "to_utc_min", msg.windows[win_num].to_utc_min);
    DictSafeSetItem(win, "cur_dir", msg.windows[win_num].cur_dir);
    DictSafeSetItem(win, "cur_speed", msg.windows[win_num].cur_speed);
    PyList_SetItem(window_list, win_num, win);
  }
  PyDict_SetItem(dict, PyUnicode_FromString("windows"), window_list);
}


PyObject*
ais6_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);
  Ais6 msg(nmea_payload, pad);
  if (msg.had_error() && msg.get_error() != AIS_UNINITIALIZED) {
    PyErr_Format(ais_py_exception, "Ais6: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", 6);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);
  DictSafeSetItem(dict, "seq", msg.seq);
  DictSafeSetItem(dict, "mmsi_dest", msg.mmsi_dest);
  DictSafeSetItem(dict, "retransmit", msg.retransmit);
  DictSafeSetItem(dict, "spare", msg.spare);
  DictSafeSetItem(dict, "dac", msg.dac);
  DictSafeSetItem(dict, "fi", msg.fi);

  // TODO(schwehr): manage all the submessage types

  // TODO(schwehr): properly handle exceptions in the sub messages!!!!
  switch (msg.dac) {
  case 1:  // IMO
    switch (msg.fi) {
      // TODO(schwehr): make this a function so that it can be followed
    case 0:  // OLD ITU 1371-1
      ais6_1_0_append_pydict(nmea_payload, dict, pad); break;
    case 1:  // OLD ITU 1371-1
      ais6_1_1_append_pydict(nmea_payload, dict, pad); break;
    case 2:  // OLD ITU 1371-1
        ais6_1_2_append_pydict(nmea_payload, dict, pad); break;
    case 3:  // OLD ITU 1371-1
      ais6_1_3_append_pydict(nmea_payload, dict, pad); break;
    case 4:  // OLD ITU 1371-1
      ais6_1_4_append_pydict(nmea_payload, dict, pad); break;
      // 5 -  6 -  7 -  8 -  9 -  10 -  11 do not exist
    case 12:  // Dangerous cargo indication - not to be used after 1 Jan 2013
      ais6_1_12_append_pydict(nmea_payload, dict, pad);  break;
      // 13
    case 14:  // Tidal window - not to be used after 1 Jan 2013
      ais6_1_14_append_pydict(nmea_payload, dict, pad); break;
      // 15 -  16 -  17 do not exist
    case 18:  //
      ais6_1_18_append_pydict(nmea_payload, dict, pad); break;
      // 19 does not exist
    case 20:  //
      ais6_1_20_append_pydict(nmea_payload, dict, pad); break;
      // 21 -  22 -  23 -  24
    case 25:  //
      ais6_1_25_append_pydict(nmea_payload, dict, pad); break;
      // 26 -  27
      // case 28:  // Route Addressed
      //   ais6_1_28_append_pydict(nmea_payload, dict, pad);
      //   break;
      // 29
      // case 30:  // TODO(schwehr): Text Addressed
      //   ais6_1_30_append_pydict(nmea_payload, dict, pad);
      //   break;
      // 31
    case 32:  // IMO Circ 289 - Tidal window
      ais6_1_32_append_pydict(nmea_payload, dict, pad); break;
    case 40:  // OLD ITU 1371-1
      ais6_1_40_append_pydict(nmea_payload, dict, pad); break;
    default:
      // TODO(schwehr): Raise an exception?
      DictSafeSetItem(dict, "not_parsed", true);
    }
    break;

    default:
      // TODO(schwehr): Raise an exception?
      DictSafeSetItem(dict, "not_parsed", true);
  }

  return dict;
}


PyObject*
ais7_13_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);
  Ais7_13 msg(nmea_payload, pad);
  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais7_13: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", msg.message_id);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);

  PyObject *list = PyList_New(msg.dest_mmsi.size());
  for (size_t i = 0; i < msg.dest_mmsi.size(); i++) {
    PyObject *tuple = PyTuple_New(2);
    PyTuple_SetItem(tuple, 0, PyLong_FromLong(msg.dest_mmsi[i]));  // Steals ref
    PyTuple_SetItem(tuple, 1, PyLong_FromLong(msg.seq_num[i]));  // Steals ref
    PyList_SetItem(list, i, tuple);  // Steals ref
  }
  PyDict_SetItem(dict, PyUnicode_FromString("acks"), list);
  Py_DECREF(list);
  return dict;
}

void
ais8_1_0_append_pydict(const char *nmea_payload, PyObject *dict,
                       const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais8_1_0 msg(nmea_payload, pad);
  if (msg.had_error())
    return;
  DictSafeSetItem(dict, "ack_required",  msg.ack_required);
  DictSafeSetItem(dict, "msg_seq",  msg.msg_seq);
  DictSafeSetItem(dict, "text", msg.text);
  DictSafeSetItem(dict, "spare2", msg.spare2);
}

// ais 8_1_[1..10] do not exist

void
ais8_1_11_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais8_1_11 msg(nmea_payload, pad);
  DictSafeSetItem(dict, "x", msg.x);
  DictSafeSetItem(dict, "y", msg.y);

  DictSafeSetItem(dict, "wind_ave", msg.wind_ave);
  DictSafeSetItem(dict, "wind_gust", msg.wind_gust);
  DictSafeSetItem(dict, "wind_dir", msg.wind_dir);
  DictSafeSetItem(dict, "wind_gust_dir", msg.wind_gust);

  DictSafeSetItem(dict, "air_temp", msg.air_temp);
  DictSafeSetItem(dict, "rel_humid", msg.rel_humid);
  DictSafeSetItem(dict, "dew_point", msg.dew_point);
  DictSafeSetItem(dict, "air_pres", msg.air_pres);
  DictSafeSetItem(dict, "air_pres_trend", msg.air_pres_trend);
  DictSafeSetItem(dict, "horz_vis", msg.horz_vis);

  DictSafeSetItem(dict, "water_level", msg.water_level);
  DictSafeSetItem(dict, "water_level_trend", msg.water_level_trend);

  DictSafeSetItem(dict, "surf_cur_speed", msg.surf_cur_speed);
  DictSafeSetItem(dict, "surf_cur_dir", msg.surf_cur_dir);

  DictSafeSetItem(dict, "cur_speed_2", msg.cur_speed_2);
  DictSafeSetItem(dict, "cur_dir_2",   msg.cur_dir_2);
  DictSafeSetItem(dict, "cur_depth_2", msg.cur_depth_2);

  DictSafeSetItem(dict, "cur_speed_3", msg.cur_speed_3);
  DictSafeSetItem(dict, "cur_dir_3",   msg.cur_dir_3);
  DictSafeSetItem(dict, "cur_depth_3", msg.cur_depth_3);

  DictSafeSetItem(dict, "wave_height", msg.wave_height);
  DictSafeSetItem(dict, "wave_period", msg.wave_period);
  DictSafeSetItem(dict, "wave_dir", msg.wave_dir);

  DictSafeSetItem(dict, "swell_height", msg.swell_height);
  DictSafeSetItem(dict, "swell_period", msg.swell_period);
  DictSafeSetItem(dict, "swell_dir", msg.swell_dir);

  DictSafeSetItem(dict, "sea_state", msg.sea_state);
  DictSafeSetItem(dict, "water_temp", msg.water_temp);
  DictSafeSetItem(dict, "precip_type", msg.precip_type);
  DictSafeSetItem(dict, "ice", msg.ice);  // Grr... ice

  // Or could be spare
  DictSafeSetItem(dict, "ext_water_level", msg.extended_water_level);
  DictSafeSetItem(dict, "spare2", msg.extended_water_level);
}

// 12 is addressed

void
ais8_1_13_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais8_1_13 msg(nmea_payload, pad);  // TODO(schwehr): check for errors
  DictSafeSetItem(dict, "reason", msg.reason);
  DictSafeSetItem(dict, "location_from", msg.location_from);
  DictSafeSetItem(dict, "location_to", msg.location_to);
  DictSafeSetItem(dict, "radius", msg.radius);
  DictSafeSetItem(dict, "units", msg.units);
  DictSafeSetItem(dict, "day_from", msg.day_from);
  DictSafeSetItem(dict, "month_from", msg.month_from);
  DictSafeSetItem(dict, "hour_from", msg.hour_from);
  DictSafeSetItem(dict, "minute_from", msg.minute_from);
  DictSafeSetItem(dict, "day_to", msg.day_to);
  DictSafeSetItem(dict, "month_to", msg.month_to);
  DictSafeSetItem(dict, "hour_to", msg.hour_to);
  DictSafeSetItem(dict, "minute_to", msg.minute_to);
  DictSafeSetItem(dict, "spare2", msg.spare2);
}

// 14 is addressed

void
ais8_1_15_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais8_1_15 msg(nmea_payload, pad);  // TODO(schwehr): check for errors
  DictSafeSetItem(dict, "air_draught", msg.air_draught);
  DictSafeSetItem(dict, "spare2", msg.spare2);
}

void
ais8_1_16_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais8_1_16 msg(nmea_payload, pad);  // TODO(schwehr): check for errors
  DictSafeSetItem(dict, "persons", msg.persons);
  DictSafeSetItem(dict, "spare2", msg.spare2);
}

void
ais8_1_17_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais8_1_17 msg(nmea_payload, pad);  // TODO(schwehr): check for errors
  PyObject *target_list = PyList_New(msg.targets.size());
  for (size_t target_num = 0; target_num < msg.targets.size(); target_num++) {
    PyObject *target = PyDict_New();
    DictSafeSetItem(target, "type", msg.targets[target_num].type);
    DictSafeSetItem(target, "id", msg.targets[target_num].id);
    DictSafeSetItem(target, "spare", msg.targets[target_num].spare);
    DictSafeSetItem(target, "x", msg.targets[target_num].x);
    DictSafeSetItem(target, "y", msg.targets[target_num].y);
    DictSafeSetItem(target, "cog", msg.targets[target_num].cog);
    DictSafeSetItem(target, "timestamp", msg.targets[target_num].timestamp);
    DictSafeSetItem(target, "sog", msg.targets[target_num].sog);
    PyList_SetItem(target_list, target_num, target);
  }
  PyDict_SetItem(dict, PyUnicode_FromString("targets"), target_list);
}

// 18 is addressed only

void
ais8_1_19_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais8_1_19 msg(nmea_payload, pad);  // TODO(schwehr): check for errors
  DictSafeSetItem(dict, "link_id", msg.link_id);
  DictSafeSetItem(dict, "name", msg.name);
  DictSafeSetItem(dict, "x", msg.x);
  DictSafeSetItem(dict, "y", msg.y);
  DictSafeSetItem(dict, "status", msg.status);
  DictSafeSetItem(dict, "signal", msg.signal);
  DictSafeSetItem(dict, "utc_hour_next", msg.utc_hour_next);
  DictSafeSetItem(dict, "utc_min_next", msg.utc_min_next);
  DictSafeSetItem(dict, "next_signal", msg.next_signal);
  DictSafeSetItem(dict, "spare2_0", msg.spare2[0]);
  DictSafeSetItem(dict, "spare2_1", msg.spare2[1]);
  DictSafeSetItem(dict, "spare2_2", msg.spare2[2]);
  DictSafeSetItem(dict, "spare2_3", msg.spare2[3]);
}

// 20 is addressed

void
ais8_1_21_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais8_1_21 msg(nmea_payload, pad);  // TODO(schwehr): check for errors

  DictSafeSetItem(dict, "x", msg.x);
  DictSafeSetItem(dict, "y", msg.y);
  DictSafeSetItem(dict, "utc_month", msg.utc_month);
  DictSafeSetItem(dict, "utc_day", msg.utc_day);
  DictSafeSetItem(dict, "utc_hour", msg.utc_hour);
  DictSafeSetItem(dict, "utc_min", msg.utc_min);

  if (0 == msg.type_wx_report) {
    // WX obs from ship
    DictSafeSetItem(dict, "location", msg.location);
    DictSafeSetItem(dict, "wx", msg.wx[0]);  // TODO(schwehr) Rename present?
    DictSafeSetItem(dict, "horz_viz", msg.horz_viz);
    DictSafeSetItem(dict, "humidity", msg.humidity);
    DictSafeSetItem(dict, "wind_speed", msg.wind_speed);
    DictSafeSetItem(dict, "wind_dir", msg.wind_dir);
    DictSafeSetItem(dict, "pressure", msg.pressure);
    DictSafeSetItem(dict, "pressure_tendency", msg.pressure_tendency);
    DictSafeSetItem(dict, "air_temp", msg.air_temp);
    DictSafeSetItem(dict, "water_temp", msg.water_temp);
    DictSafeSetItem(dict, "wave_period", msg.wave_period);
    DictSafeSetItem(dict, "wave_height", msg.wave_height);
    DictSafeSetItem(dict, "wave_dir", msg.wave_dir);
    DictSafeSetItem(dict, "swell_height", msg.swell_height);
    DictSafeSetItem(dict, "swell_dir", msg.swell_dir);
    DictSafeSetItem(dict, "swell_period", msg.swell_period);
    DictSafeSetItem(dict, "spare2", msg.spare2);
  } else {
    // type == 1
    // PAIN IN THE ASS WMO OBS from ship
    DictSafeSetItem(dict, "cog", msg.cog);
    DictSafeSetItem(dict, "sog", msg.sog);
    DictSafeSetItem(dict, "heading", msg.heading);
    DictSafeSetItem(dict, "pressure", msg.pressure);
    DictSafeSetItem(dict, "rel_pressure", msg.rel_pressure);
    DictSafeSetItem(dict, "pressure_tendency", msg.pressure_tendency);
    DictSafeSetItem(dict, "wind_dir", msg.wind_dir);
    DictSafeSetItem(dict, "wind_speed_ms", msg.wind_speed_ms);
    DictSafeSetItem(dict, "wind_dir_rel", msg.wind_dir_rel);
    DictSafeSetItem(dict, "wind_speed_rel", msg.wind_speed_rel);
    DictSafeSetItem(dict, "wind_gust_speed", msg.wind_gust_speed);
    DictSafeSetItem(dict, "wind_gust_dir", msg.wind_gust_dir);
    DictSafeSetItem(dict, "air_temp_raw", msg.air_temp_raw);
    DictSafeSetItem(dict, "humidity", msg.humidity);
    DictSafeSetItem(dict, "water_temp_raw", msg.water_temp_raw);
    DictSafeSetItem(dict, "horz_viz", msg.horz_viz);
    // TODO(schwehr): list?
    DictSafeSetItem(dict, "wx", msg.wx[0]);
    DictSafeSetItem(dict, "wx_next1", msg.wx[1]);
    DictSafeSetItem(dict, "wx_next2", msg.wx[2]);
    DictSafeSetItem(dict, "cloud_total", msg.cloud_total);
    DictSafeSetItem(dict, "cloud_low", msg.cloud_low);
    DictSafeSetItem(dict, "cloud_low_type", msg.cloud_low_type);
    DictSafeSetItem(dict, "cloud_middle_type", msg.cloud_middle_type);
    DictSafeSetItem(dict, "cloud_high_type", msg.cloud_high_type);
    DictSafeSetItem(dict, "alt_lowest_cloud_base", msg.alt_lowest_cloud_base);
    DictSafeSetItem(dict, "wave_period", msg.wave_period);
    DictSafeSetItem(dict, "wave_height", msg.wave_height);
    DictSafeSetItem(dict, "swell_dir", msg.swell_dir);
    DictSafeSetItem(dict, "swell_period", msg.swell_period);
    DictSafeSetItem(dict, "swell_height", msg.swell_height);
    DictSafeSetItem(dict, "swell_dir_2", msg.swell_dir_2);
    DictSafeSetItem(dict, "swell_period_2", msg.swell_period_2);
    DictSafeSetItem(dict, "swell_height_2", msg.swell_height_2);
    DictSafeSetItem(dict, "ice_thickness", msg.ice_thickness);
    DictSafeSetItem(dict, "ice_accretion", msg.ice_accretion);
    DictSafeSetItem(dict, "ice_accretion_cause", msg.ice_accretion_cause);
    DictSafeSetItem(dict, "sea_ice_concentration", msg.sea_ice_concentration);
    DictSafeSetItem(dict, "amt_type_ice", msg.amt_type_ice);
    DictSafeSetItem(dict, "ice_situation", msg.ice_situation);
    DictSafeSetItem(dict, "ice_devel", msg.ice_devel);
    DictSafeSetItem(dict, "bearing_ice_edge", msg.bearing_ice_edge);
  }
}


void
ais8_1_22_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  Ais8_001_22 msg(nmea_payload, pad);  // TODO(schwehr): check for errors

  DictSafeSetItem(dict, "link_id", msg.link_id);
  DictSafeSetItem(dict, "notice_type", msg.notice_type);
  DictSafeSetItem(dict, "notice_type_str",
                  ais8_001_22_notice_names[msg.notice_type]);

  DictSafeSetItem(dict, "month", msg.month);  // This is UTC, not local time.
  DictSafeSetItem(dict, "day", msg.day);
  DictSafeSetItem(dict, "hour", msg.hour);
  DictSafeSetItem(dict, "minute", msg.minute);

  PyObject *sub_area_list = PyList_New(msg.sub_areas.size());

  // Loop over sub_areas
  for (size_t i = 0; i < msg.sub_areas.size(); i++) {
    switch (msg.sub_areas[i]->getType()) {
    case AIS8_001_22_SHAPE_CIRCLE:  // or point
      {
        PyObject *sub_area = PyDict_New();
        Ais8_001_22_Circle *c =
            reinterpret_cast<Ais8_001_22_Circle*>(msg.sub_areas[i]);

        DictSafeSetItem(sub_area, "sub_area_type", AIS8_001_22_SHAPE_CIRCLE);
        if (c->radius_m == 0)
          DictSafeSetItem(sub_area, "sub_area_type_str", "point");
        else
          DictSafeSetItem(sub_area, "sub_area_type_str", "circle");

        DictSafeSetItem(sub_area, "x", c->x);
        DictSafeSetItem(sub_area, "y", c->y);
        DictSafeSetItem(sub_area, "precision", c->precision);
        DictSafeSetItem(sub_area, "radius", c->radius_m);
        // TODO(schwehr): spare?
        PyList_SetItem(sub_area_list, i, sub_area);
      }
      break;
    case AIS8_001_22_SHAPE_RECT:
      {
        PyObject *sub_area = PyDict_New();
        Ais8_001_22_Rect *c =
            reinterpret_cast<Ais8_001_22_Rect*>(msg.sub_areas[i]);

        DictSafeSetItem(sub_area, "sub_area_type", AIS8_001_22_SHAPE_RECT);
        DictSafeSetItem(sub_area, "sub_area_type_str", "rect");

        DictSafeSetItem(sub_area, "x", c->x);
        DictSafeSetItem(sub_area, "y", c->y);
        DictSafeSetItem(sub_area, "precision", c->precision);
        DictSafeSetItem(sub_area, "e_dim_m", c->e_dim_m);
        DictSafeSetItem(sub_area, "n_dim_m", c->n_dim_m);
        DictSafeSetItem(sub_area, "orient_deg", c->orient_deg);
        // TODO(schwehr): spare?
        PyList_SetItem(sub_area_list, i, sub_area);
      }
      break;
    case AIS8_001_22_SHAPE_SECTOR:
      {
        PyObject *sub_area = PyDict_New();
        Ais8_001_22_Sector *c =
            reinterpret_cast<Ais8_001_22_Sector*>(msg.sub_areas[i]);

        DictSafeSetItem(sub_area, "sub_area_type", AIS8_001_22_SHAPE_SECTOR);
        DictSafeSetItem(sub_area, "sub_area_type_str", "sector");

        DictSafeSetItem(sub_area, "x", c->x);
        DictSafeSetItem(sub_area, "y", c->y);
        DictSafeSetItem(sub_area, "precision", c->precision);
        DictSafeSetItem(sub_area, "radius", c->radius_m);
        DictSafeSetItem(sub_area, "left_bound_deg", c->left_bound_deg);
        DictSafeSetItem(sub_area, "right_bound_deg", c->right_bound_deg);
        // TODO(schwehr): spare?
        PyList_SetItem(sub_area_list, i, sub_area);
      }
      break;
    case AIS8_001_22_SHAPE_POLYLINE:
      {
        PyObject *sub_area = PyDict_New();
        Ais8_001_22_Polyline *polyline =
            reinterpret_cast<Ais8_001_22_Polyline*>(msg.sub_areas[i]);

        DictSafeSetItem(sub_area, "sub_area_type", AIS8_001_22_SHAPE_POLYLINE);
        DictSafeSetItem(sub_area, "sub_area_type_str", "polyline");
        assert(polyline->angles.size() == polyline->dists_m.size());
        PyObject *angle_list = PyList_New(polyline->angles.size());
        PyObject *dist_list = PyList_New(polyline->angles.size());

        for (size_t pt_num = 0; pt_num < polyline->angles.size(); pt_num++) {
          PyList_SetItem(angle_list, pt_num,
                         PyFloat_FromDouble(polyline->angles[pt_num]));
          PyList_SetItem(dist_list, pt_num,
                         PyFloat_FromDouble(polyline->dists_m[pt_num]));
        }

        DictSafeSetItem(sub_area, "angles", angle_list);
        DictSafeSetItem(sub_area, "dists_m", dist_list);

        // TODO(schwehr): spare?
        PyList_SetItem(sub_area_list, i, sub_area);
      }
      break;
    case AIS8_001_22_SHAPE_POLYGON:
      {
        PyObject *sub_area = PyDict_New();
        Ais8_001_22_Polygon *polygon =
            reinterpret_cast<Ais8_001_22_Polygon*>(msg.sub_areas[i]);

        DictSafeSetItem(sub_area, "sub_area_type", AIS8_001_22_SHAPE_POLYGON);
        DictSafeSetItem(sub_area, "sub_area_type_str", "polygon");
        assert(polygon->angles.size() == polygon->dists_m.size());
        PyObject *angle_list = PyList_New(polygon->angles.size());
        PyObject *dist_list = PyList_New(polygon->angles.size());

        for (size_t pt_num = 0; pt_num < polygon->angles.size(); pt_num++) {
          PyList_SetItem(angle_list, pt_num,
                         PyFloat_FromDouble(polygon->angles[pt_num]));
          PyList_SetItem(dist_list, pt_num,
                         PyFloat_FromDouble(polygon->dists_m[pt_num]));
        }

        DictSafeSetItem(sub_area, "angles", angle_list);
        DictSafeSetItem(sub_area, "dists_m", dist_list);

        // TODO(schwehr): spare?
        PyList_SetItem(sub_area_list, i, sub_area);
      }
      break;
    case AIS8_001_22_SHAPE_TEXT:
      {
        PyObject *sub_area = PyDict_New();

        Ais8_001_22_Text *text =
            reinterpret_cast<Ais8_001_22_Text*>(msg.sub_areas[i]);
        DictSafeSetItem(sub_area, "sub_area_type", AIS8_001_22_SHAPE_TEXT);
        DictSafeSetItem(sub_area, "sub_area_type_str", "text");

        DictSafeSetItem(sub_area, "text", text->text);
        PyList_SetItem(sub_area_list, i, sub_area);
      }
      break;

    default:
      {}  // TODO(schwehr): Mark an unknown subarea or raise an exception.
    }
  }
  DictSafeSetItem(dict, "sub_areas", sub_area_list);
}

// no 23 broadcast

// IMO Circ 289 - Extended ship static and voyage-related
void
ais8_1_24_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais8_1_24 msg(nmea_payload, pad);  // TODO(schwehr): check for errors
  DictSafeSetItem(dict, "link_id", msg.link_id);
  DictSafeSetItem(dict, "air_draught", msg.air_draught);
  DictSafeSetItem(dict, "last_port", msg.last_port);

  PyObject *port_list = PyList_New(2);
  PyObject *port0 = PyUnicode_FromString(msg.next_ports[0].c_str());
  PyObject *port1 = PyUnicode_FromString(msg.next_ports[0].c_str());
  PyList_SetItem(port_list, 0, port0); Py_DECREF(port0);
  PyList_SetItem(port_list, 1, port1); Py_DECREF(port1);

  PyObject *solas_list = PyList_New(26);
  for (size_t solas_num = 0; solas_num < 26; solas_num++) {
    PyObject *solas = PyInt_FromLong(msg.solas_status[solas_num]);
    PyList_SetItem(solas_list, solas_num, solas);
  }
  DictSafeSetItem(dict, "solas", solas_list);
  DictSafeSetItem(dict, "ice_class", msg.ice_class);
  DictSafeSetItem(dict, "shaft_power", msg.shaft_power);
  DictSafeSetItem(dict, "vhf", msg.vhf);
  DictSafeSetItem(dict, "lloyds_ship_type", msg.lloyds_ship_type);
  DictSafeSetItem(dict, "gross_tonnage", msg.gross_tonnage);
  DictSafeSetItem(dict, "laden_ballast", msg.laden_ballast);
  DictSafeSetItem(dict, "heavy_oil", msg.heavy_oil);
  DictSafeSetItem(dict, "light_oil", msg.light_oil);
  DictSafeSetItem(dict, "diesel", msg.diesel);
  DictSafeSetItem(dict, "bunker_oil", msg.bunker_oil);
  DictSafeSetItem(dict, "persons", msg.persons);
  DictSafeSetItem(dict, "spare2", msg.spare2);
}

// no 25 broadcast

static void
ais8_1_26_append_pydict_sensor_hdr(PyObject *dict,
                                   Ais8_1_26_SensorReport* rpt) {
  assert(dict);
  assert(rpt);
  DictSafeSetItem(dict, "report_type", rpt->report_type);
  DictSafeSetItem(dict, "utc_day", rpt->utc_day);
  DictSafeSetItem(dict, "utc_hr", rpt->utc_hr);
  DictSafeSetItem(dict, "utc_min", rpt->utc_min);
  DictSafeSetItem(dict, "site_id", rpt->site_id);
}

// IMO Circ 289 - Environmental
void
ais8_1_26_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais8_1_26 msg(nmea_payload, pad);  // TODO(schwehr): check for errors

  PyObject *rpt_list = PyList_New(msg.reports.size());
  DictSafeSetItem(dict, "reports", rpt_list);

  for (size_t rpt_num = 0; rpt_num < msg.reports.size(); rpt_num++) {
    PyObject *rpt_dict = PyDict_New();
    PyList_SetItem(rpt_list, rpt_num, rpt_dict);

    switch (msg.reports[rpt_num]->report_type) {
      // case AIS8_1_26_SENSOR_ERROR:
    case AIS8_1_26_SENSOR_LOCATION:
      {
        Ais8_1_26_Location *rpt =
            reinterpret_cast<Ais8_1_26_Location *>(msg.reports[rpt_num]);
        ais8_1_26_append_pydict_sensor_hdr(rpt_dict, rpt);
        DictSafeSetItem(rpt_dict, "x", rpt->x);
        DictSafeSetItem(rpt_dict, "y", rpt->y);
        DictSafeSetItem(rpt_dict, "z", rpt->z);
        DictSafeSetItem(rpt_dict, "owner", rpt->owner);
        DictSafeSetItem(rpt_dict, "timeout", rpt->timeout);
        DictSafeSetItem(rpt_dict, "spare", rpt->spare);
      }
      break;
    case AIS8_1_26_SENSOR_STATION:
      {
        Ais8_1_26_Station *rpt =
            reinterpret_cast<Ais8_1_26_Station *>(msg.reports[rpt_num]);
        DictSafeSetItem(rpt_dict, "name", rpt->name);
        DictSafeSetItem(rpt_dict, "spare", rpt->spare);
      }
      break;
    case AIS8_1_26_SENSOR_WIND:
      {
        Ais8_1_26_Wind *rpt =
            reinterpret_cast<Ais8_1_26_Wind *>(msg.reports[rpt_num]);
        DictSafeSetItem(rpt_dict, "wind_speed", rpt->wind_speed);
        DictSafeSetItem(rpt_dict, "wind_gust", rpt->wind_gust);
        DictSafeSetItem(rpt_dict, "wind_dir", rpt->wind_dir);
        DictSafeSetItem(rpt_dict, "wind_gust_dir", rpt->wind_gust_dir);
        DictSafeSetItem(rpt_dict, "sensor_type", rpt->sensor_type);
        DictSafeSetItem(rpt_dict, "wind_forcast", rpt->wind_forcast);
        DictSafeSetItem(rpt_dict, "wind_gust_forcast", rpt->wind_gust_forcast);
        DictSafeSetItem(rpt_dict, "wind_dir_forcast", rpt->wind_dir_forcast);
        DictSafeSetItem(rpt_dict, "utc_day_forcast", rpt->utc_day_forcast);
        DictSafeSetItem(rpt_dict, "utc_hour_forcast", rpt->utc_hour_forcast);
        DictSafeSetItem(rpt_dict, "utc_min_forcast", rpt->utc_min_forcast);
        DictSafeSetItem(rpt_dict, "duration", rpt->duration);
        DictSafeSetItem(rpt_dict, "spare", rpt->spare);
      }
      break;
    case AIS8_1_26_SENSOR_WATER_LEVEL:
      {
        Ais8_1_26_WaterLevel *rpt =
            reinterpret_cast<Ais8_1_26_WaterLevel *>(msg.reports[rpt_num]);
        DictSafeSetItem(rpt_dict, "type", rpt->type);
        DictSafeSetItem(rpt_dict, "level", rpt->level);
        DictSafeSetItem(rpt_dict, "trend", rpt->trend);
        DictSafeSetItem(rpt_dict, "vdatum", rpt->vdatum);
        DictSafeSetItem(rpt_dict, "sensor_type", rpt->sensor_type);
        DictSafeSetItem(rpt_dict, "forcast_type", rpt->forcast_type);
        DictSafeSetItem(rpt_dict, "level_forcast", rpt->level_forcast);
        DictSafeSetItem(rpt_dict, "utc_day_forcast", rpt->utc_day_forcast);
        DictSafeSetItem(rpt_dict, "utc_hour_forcast", rpt->utc_hour_forcast);
        DictSafeSetItem(rpt_dict, "utc_min_forcast", rpt->utc_min_forcast);
        DictSafeSetItem(rpt_dict, "duration", rpt->duration);
        DictSafeSetItem(rpt_dict, "spare", rpt->spare);
      }
      break;
    case AIS8_1_26_SENSOR_CURR_2D:
      {
        Ais8_1_26_Curr2D *rpt =
            reinterpret_cast<Ais8_1_26_Curr2D *>(msg.reports[rpt_num]);
        DictSafeSetItem(rpt_dict, "type", rpt->type);
        DictSafeSetItem(rpt_dict, "spare", rpt->spare);

        PyObject *curr_list = PyList_New(3);
        DictSafeSetItem(dict, "currents", curr_list);
        for (size_t idx = 0; idx < 3; idx++) {
          PyObject *curr_dict = PyDict_New();
          DictSafeSetItem(curr_dict, "speed", rpt->currents[idx].speed);
          DictSafeSetItem(curr_dict, "dir", rpt->currents[idx].dir);
          DictSafeSetItem(curr_dict, "depth", rpt->currents[idx].depth);
          PyList_SetItem(curr_list, idx, curr_dict);
        }
      }
      break;
    case AIS8_1_26_SENSOR_CURR_3D:
      {
        Ais8_1_26_Curr3D *rpt =
            reinterpret_cast<Ais8_1_26_Curr3D *>(msg.reports[rpt_num]);
        DictSafeSetItem(rpt_dict, "type", rpt->type);
        DictSafeSetItem(rpt_dict, "spare", rpt->spare);

        PyObject *curr_list = PyList_New(3);
        DictSafeSetItem(dict, "currents", curr_list);
        for (size_t idx = 0; idx < 2; idx++) {
          // ERROR: no way to specify negative direction
          PyObject *curr_dict = PyDict_New();
          PyList_SetItem(curr_list, idx, curr_dict);
          DictSafeSetItem(curr_dict, "north", rpt->currents[idx].north);
          DictSafeSetItem(curr_dict, "east", rpt->currents[idx].east);
          DictSafeSetItem(curr_dict, "up", rpt->currents[idx].up);
          DictSafeSetItem(curr_dict, "depth", rpt->currents[idx].depth);
        }
      }
      break;
    case AIS8_1_26_SENSOR_HORZ_FLOW:
      {
        Ais8_1_26_HorzFlow *rpt =
            reinterpret_cast<Ais8_1_26_HorzFlow *>(msg.reports[rpt_num]);
        DictSafeSetItem(rpt_dict, "spare", rpt->spare);

        PyObject *curr_list = PyList_New(3);
        DictSafeSetItem(dict, "currents", curr_list);
        for (size_t idx = 0; idx < 2; idx++) {
          PyObject *curr_dict = PyDict_New();
          PyList_SetItem(curr_list, idx, curr_dict);
          DictSafeSetItem(curr_dict, "bearing", rpt->currents[idx].bearing);
          DictSafeSetItem(curr_dict, "dist", rpt->currents[idx].dist);
          DictSafeSetItem(curr_dict, "speed", rpt->currents[idx].speed);
          DictSafeSetItem(curr_dict, "dir", rpt->currents[idx].dir);
          DictSafeSetItem(curr_dict, "level", rpt->currents[idx].level);
        }
      }
      break;
    case AIS8_1_26_SENSOR_SEA_STATE:
      {
        Ais8_1_26_SeaState *rpt =
            reinterpret_cast<Ais8_1_26_SeaState *>(msg.reports[rpt_num]);
        DictSafeSetItem(rpt_dict, "swell_height", rpt->swell_height);
        DictSafeSetItem(rpt_dict, "swell_period", rpt->swell_period);
        DictSafeSetItem(rpt_dict, "swell_dir", rpt->swell_dir);
        DictSafeSetItem(rpt_dict, "sea_state", rpt->sea_state);
        DictSafeSetItem(rpt_dict, "swell_sensor_type", rpt->swell_sensor_type);
        DictSafeSetItem(rpt_dict, "water_temp", rpt->water_temp);
        DictSafeSetItem(rpt_dict, "water_temp_depth", rpt->water_temp_depth);
        DictSafeSetItem(rpt_dict, "water_sensor_type", rpt->water_sensor_type);
        DictSafeSetItem(rpt_dict, "wave_height", rpt->wave_height);
        DictSafeSetItem(rpt_dict, "wave_period", rpt->wave_period);
        DictSafeSetItem(rpt_dict, "wave_dir", rpt->wave_dir);
        DictSafeSetItem(rpt_dict, "wave_sensor_type", rpt->wave_sensor_type);
        DictSafeSetItem(rpt_dict, "salinity", rpt->salinity);
      }
      break;
    case AIS8_1_26_SENSOR_SALINITY:
      {
        Ais8_1_26_Salinity *rpt =
            reinterpret_cast<Ais8_1_26_Salinity *>(msg.reports[rpt_num]);
        DictSafeSetItem(rpt_dict, "water_temp", rpt->water_temp);
        DictSafeSetItem(rpt_dict, "conductivity", rpt->conductivity);
        DictSafeSetItem(rpt_dict, "pressure", rpt->pressure);
        DictSafeSetItem(rpt_dict, "salinity", rpt->salinity);
        DictSafeSetItem(rpt_dict, "salinity_type", rpt->salinity_type);
        DictSafeSetItem(rpt_dict, "sensor_type", rpt->sensor_type);
        DictSafeSetItem(rpt_dict, "spare0", rpt->spare[0]);
        DictSafeSetItem(rpt_dict, "spare1", rpt->spare[1]);
      }
      break;
    case AIS8_1_26_SENSOR_WX:
      {
        Ais8_1_26_Wx *rpt =
            reinterpret_cast<Ais8_1_26_Wx *>(msg.reports[rpt_num]);
        DictSafeSetItem(rpt_dict, "air_temp", rpt->air_temp);
        DictSafeSetItem(rpt_dict, "air_temp_sensor_type",
                        rpt->air_temp_sensor_type);
        DictSafeSetItem(rpt_dict, "precip", rpt->precip);
        DictSafeSetItem(rpt_dict, "horz_vis", rpt->horz_vis);
        DictSafeSetItem(rpt_dict, "dew_point", rpt->dew_point);
        DictSafeSetItem(rpt_dict, "dew_point_type", rpt->dew_point_type);
        DictSafeSetItem(rpt_dict, "air_pressure", rpt->air_pressure);
        DictSafeSetItem(rpt_dict, "air_pressure_trend",
                        rpt->air_pressure_trend);
        DictSafeSetItem(rpt_dict, "air_pressor_type", rpt->air_pressor_type);
        DictSafeSetItem(rpt_dict, "salinity", rpt->salinity);
        DictSafeSetItem(rpt_dict, "spare", rpt->spare);
      }
      break;
    case AIS8_1_26_SENSOR_AIR_DRAUGHT:
      {
        Ais8_1_26_AirDraught *rpt =
            reinterpret_cast<Ais8_1_26_AirDraught *>(msg.reports[rpt_num]);
        DictSafeSetItem(rpt_dict, "draught", rpt->draught);
        DictSafeSetItem(rpt_dict, "gap", rpt->gap);
        DictSafeSetItem(rpt_dict, "forcast_gap", rpt->forcast_gap);
        DictSafeSetItem(rpt_dict, "int trend", rpt->trend);
        DictSafeSetItem(rpt_dict, "int utc_day_forcast", rpt->utc_day_forcast);
        DictSafeSetItem(rpt_dict, "utc_hour_forcast", rpt->utc_hour_forcast);
        DictSafeSetItem(rpt_dict, "utc_min_forcast", rpt->utc_min_forcast);
        DictSafeSetItem(rpt_dict, "spare", rpt->spare);
      }
      break;
    case AIS8_1_26_SENSOR_RESERVED_11:  // FALLTHROUGH
    case AIS8_1_26_SENSOR_RESERVED_12:
    case AIS8_1_26_SENSOR_RESERVED_13:
    case AIS8_1_26_SENSOR_RESERVED_14:
    case AIS8_1_26_SENSOR_RESERVED_15:
    default:
      {}  // TODO(schwehr): mark a bad sensor type or raise exception
    }
  }
}


// IMO Circ 289 - Route information
void
ais8_1_27_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais8_1_27 msg(nmea_payload, pad);  // TODO(schwehr): check for errors
  DictSafeSetItem(dict, "link_id", msg.link_id);
  DictSafeSetItem(dict, "sender_type", msg.sender_type);
  DictSafeSetItem(dict, "route_type", msg.route_type);
  DictSafeSetItem(dict, "utc_month", msg.utc_month);
  DictSafeSetItem(dict, "utc_day", msg.utc_day);
  DictSafeSetItem(dict, "utc_hour", msg.utc_hour);
  DictSafeSetItem(dict, "utc_min", msg.utc_min);
  DictSafeSetItem(dict, "duration", msg.duration);

  PyObject *waypoint_list = PyList_New(msg.waypoints.size());
  for (size_t point_num = 0; point_num < msg.waypoints.size(); point_num++) {
    PyObject *waypoint = PyList_New(2);
    // TODO(schwehr): Py_DECREF(); ?
    PyList_SetItem(waypoint, 0, PyFloat_FromDouble(msg.waypoints[point_num].x));
    PyList_SetItem(waypoint, 1, PyFloat_FromDouble(msg.waypoints[point_num].y));
    PyList_SetItem(waypoint_list, point_num, waypoint);
  }
  PyDict_SetItem(dict, PyUnicode_FromString("waypoints"), waypoint_list);
}

// no 28 broadcast

// IMO Circ 289 - Text description
void
ais8_1_29_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais8_1_29 msg(nmea_payload, pad);  // TODO(schwehr): check for errors

  DictSafeSetItem(dict, "link_id", msg.link_id);
  DictSafeSetItem(dict, "text", msg.text);
}

// no 30 broadcast

void
ais8_1_31_append_pydict(const char *nmea_payload, PyObject *dict,
                        const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais8_1_31 msg(nmea_payload, pad);  // TODO(schwehr): check for errors

  DictSafeSetItem(dict, "x", msg.x);
  DictSafeSetItem(dict, "y", msg.y);
  DictSafeSetItem(dict, "position_accuracy", msg.position_accuracy);
  DictSafeSetItem(dict, "utc_day", msg.utc_day);
  DictSafeSetItem(dict, "utc_hour", msg.utc_hour);
  DictSafeSetItem(dict, "utc_min", msg.utc_min);
  DictSafeSetItem(dict, "wind_ave", msg.wind_ave);
  DictSafeSetItem(dict, "wind_gust", msg.wind_gust);
  DictSafeSetItem(dict, "wind_dir", msg.wind_dir);
  DictSafeSetItem(dict, "wind_gust_dir", msg.wind_gust_dir);
  DictSafeSetItem(dict, "air_temp", msg.air_temp);
  DictSafeSetItem(dict, "rel_humid", msg.rel_humid);
  DictSafeSetItem(dict, "dew_point", msg.dew_point);
  DictSafeSetItem(dict, "air_pres", msg.air_pres);
  DictSafeSetItem(dict, "air_pres_trend", msg.air_pres_trend);
  DictSafeSetItem(dict, "horz_vis", msg.horz_vis);
  DictSafeSetItem(dict, "water_level", msg.water_level);
  DictSafeSetItem(dict, "water_level_trend", msg.water_level_trend);

  // TODO(schwehr): make this a list of dicts
  DictSafeSetItem(dict, "surf_cur_speed", msg.surf_cur_speed);
  DictSafeSetItem(dict, "surf_cur_dir", msg.surf_cur_dir);
  DictSafeSetItem(dict, "cur_speed_2", msg.cur_speed_2);
  DictSafeSetItem(dict, "cur_dir_2", msg.cur_dir_2);
  DictSafeSetItem(dict, "cur_depth_2", msg.cur_depth_2);
  DictSafeSetItem(dict, "cur_speed_3", msg.cur_speed_3);
  DictSafeSetItem(dict, "cur_dir_3", msg.cur_dir_3);
  DictSafeSetItem(dict, "cur_depth_3", msg.cur_depth_3);

  DictSafeSetItem(dict, "wave_height", msg.wave_height);
  DictSafeSetItem(dict, "wave_period", msg.wave_period);
  DictSafeSetItem(dict, "wave_dir", msg.wave_dir);
  DictSafeSetItem(dict, "swell_height", msg.swell_height);
  DictSafeSetItem(dict, "swell_period", msg.swell_period);
  DictSafeSetItem(dict, "swell_dir", msg.swell_dir);
  DictSafeSetItem(dict, "sea_state", msg.sea_state);
  DictSafeSetItem(dict, "water_temp", msg.water_temp);
  DictSafeSetItem(dict, "precip_type", msg.precip_type);
  DictSafeSetItem(dict, "salinity", msg.salinity);
  DictSafeSetItem(dict, "ice", msg.ice);
}

// no 32 broadcast

// DAC 200 - River Information System
void
ais8_200_10_append_pydict(const char *nmea_payload, PyObject *dict,
                          const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais8_200_10 msg(nmea_payload, pad);
  if (msg.had_error()) return;   // TODO(schwehr): handle errors

  DictSafeSetItem(dict, "eu_id", msg.eu_id);
  DictSafeSetItem(dict, "length", msg.length);
  DictSafeSetItem(dict, "beam", msg.beam);
  DictSafeSetItem(dict, "ship_type", msg.ship_type);
  DictSafeSetItem(dict, "haz_cargo", msg.haz_cargo);
  DictSafeSetItem(dict, "draught", msg.draught);
  DictSafeSetItem(dict, "loaded", msg.loaded);
  DictSafeSetItem(dict, "speed_qual", msg.speed_qual);
  DictSafeSetItem(dict, "course_qual", msg.course_qual);
  DictSafeSetItem(dict, "heading_qual", msg.heading_qual);
  DictSafeSetItem(dict, "spare2", msg.spare2);
}

// River Information System
void
ais8_200_23_append_pydict(const char *nmea_payload, PyObject *dict,
                          const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais8_200_23 msg(nmea_payload, pad);
  if (msg.had_error()) return;   // TODO(schwehr): handle errors correctly

  DictSafeSetItem(dict, "utc_year_start", msg.utc_year_start);
  DictSafeSetItem(dict, "utc_month_start", msg.utc_month_start);
  DictSafeSetItem(dict, "utc_day_start", msg.utc_day_start);
  DictSafeSetItem(dict, "utc_year_end", msg.utc_year_end);
  DictSafeSetItem(dict, "utc_month_end", msg.utc_month_end);
  DictSafeSetItem(dict, "utc_day_end", msg.utc_day_end);
  DictSafeSetItem(dict, "utc_hour_start", msg.utc_hour_start);
  DictSafeSetItem(dict, "utc_min_start", msg.utc_min_start);
  DictSafeSetItem(dict, "utc_hour_end", msg.utc_hour_end);
  DictSafeSetItem(dict, "utc_min_end", msg.utc_min_end);
  DictSafeSetItem(dict, "x1", msg.x1);
  DictSafeSetItem(dict, "y1", msg.y1);
  DictSafeSetItem(dict, "x2", msg.x2);
  DictSafeSetItem(dict, "y2", msg.y2);
  DictSafeSetItem(dict, "type", msg.type);
  DictSafeSetItem(dict, "min", msg.min);
  DictSafeSetItem(dict, "max", msg.max);
  DictSafeSetItem(dict, "classification", msg.classification);
  DictSafeSetItem(dict, "wind_dir", msg.wind_dir);
  DictSafeSetItem(dict, "spare2", msg.spare2);
}

// River Information System
void
ais8_200_24_append_pydict(const char *nmea_payload, PyObject *dict,
                          const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais8_200_24 msg(nmea_payload, pad);
  if (msg.had_error())
    return;   // TODO(schwehr): handle errors correctly
  DictSafeSetItem(dict, "country", msg.country);

  PyObject *id_list = PyList_New(4);
  for (size_t i = 0; i < 4; i++)
    PyList_SetItem(id_list, 0, PyInt_FromLong(msg.guage_ids[i]));
  DictSafeSetItem(dict, "guage_ids", id_list);

  PyObject *level_list = PyList_New(4);
  for (size_t i = 0; i < 4; i++)
    PyList_SetItem(level_list, 0, PyFloat_FromDouble(msg.levels[i]));
  DictSafeSetItem(dict, "levels", level_list);
}

// River Information System
void
ais8_200_40_append_pydict(const char *nmea_payload, PyObject *dict,
                          const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais8_200_40 msg(nmea_payload, pad);
  if (msg.had_error())
    return;   // TODO(schwehr): handle errors correctly
  DictSafeSetItem(dict, "x", msg.x);
  DictSafeSetItem(dict, "y", msg.y);
  DictSafeSetItem(dict, "form", msg.form);
  DictSafeSetItem(dict, "dir", msg.dir);
  DictSafeSetItem(dict, "stream_dir", msg.stream_dir);
  DictSafeSetItem(dict, "status_raw", msg.status_raw);
  // TODO(schwehr): extract status components
  DictSafeSetItem(dict, "spare2", msg.spare2);
}

// River Information System
void
ais8_200_55_append_pydict(const char *nmea_payload, PyObject *dict,
                          const size_t pad) {
  assert(nmea_payload);
  assert(dict);
  assert(pad < 6);
  Ais8_200_55 msg(nmea_payload, pad);
  if (msg.had_error())
    return;   // TODO(schwehr): handle errors correctly
  DictSafeSetItem(dict, "crew", msg.crew);
  DictSafeSetItem(dict, "passengers", msg.passengers);
  DictSafeSetItem(dict, "yet_more_personnel", msg.yet_more_personnel);

  PyObject *spare2_list = PyList_New(3);
  for (size_t i = 0; i < 3; i++)
    PyList_SetItem(spare2_list, 0,  PyInt_FromLong(msg.spare2[i]));
  DictSafeSetItem(dict, "spare2", spare2_list);
}


// AIS Binary broadcast messages.  There will be a huge number of subtypes
// If we don't know how to decode it, just return the dac, fi
PyObject*
ais8_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);
  assert(pad < 6);

  Ais8 msg(nmea_payload, pad);
  if (msg.had_error() && msg.get_error() != AIS_UNINITIALIZED) {
    PyErr_Format(ais_py_exception, "Ais8: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", 8);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);
  DictSafeSetItem(dict, "spare", msg.spare);
  DictSafeSetItem(dict, "dac", msg.dac);
  DictSafeSetItem(dict, "fi", msg.fi);

  // TODO(schwehr): better error handling
  switch (msg.dac) {
  case 1:  // IMO
    switch (msg.fi) {
      // See: ITU-R.M.1371-3 IFM messages Annex 5, Section 5 and IMO Circ 289
    case 0:  // Text using 6 bit ascii
      ais8_1_0_append_pydict(nmea_payload, dict, pad);
      break;

      // 1 is listed in ITU 1371-1 as addressed only.  Not listed in 1371-3
      // 1 is "Application acknoldegement"
      // 2 Interrogation for specific FM (ABM).  Addressed only
      // 3 Capability interrogation.  Addressed only
      // 4. Capability reply to 3.  Addressed only

      //
      // IMO Circ 289
      //
      // TODO(schwehr): error checking
    case 11:  // Met/Hydrogrolocal - not to be used after 1 Jan 2013
      ais8_1_11_append_pydict(nmea_payload, dict, pad);
      break;
      // 12 is an ABM
    case 13:  // Fairway closed - not to be used after 1 Jan 2013
      // TODO(schwehr): untested - no messages found
      ais8_1_13_append_pydict(nmea_payload, dict, pad);
      break;
      // No 14
    case 15:  // Extended ship static and voyage - not after 1 Jan 2013
      // TODO(schwehr): untested - no messages found
      ais8_1_15_append_pydict(nmea_payload, dict, pad);
      break;
      // 16 has conflicting definition in the old 1371-1: VTS targets
    case 16:  // Number of persons on board - not to be used after 1 Jan 2013
      ais8_1_16_append_pydict(nmea_payload, dict, pad);
      break;
      // 17 has conflicting definitions in 1371-1: IFM 17: Ship waypoints (WP)
      // and/or route plan report
    case 17:  // VTS-generated/synthetic targets - not to be used after Jan 2013
      // TODO(schwehr): untested - no messages found
      ais8_1_17_append_pydict(nmea_payload, dict, pad);
      break;
      // No 18
      // ITU 1371-1 conflict: IFM 19: Extended ship static and voyage
    case 19:  // Marine traffic signal
      // TODO(schwehr): untested - no messages found
      ais8_1_19_append_pydict(nmea_payload, dict, pad);
      DictSafeSetItem(dict, "parsed", true);
      break;
      // case 20:  // Berthing data
      //    ais8_1_20_append_pydict(nmea_payload, dict, pad);
      //    DictSafeSetItem(dict, "parsed",true);
      //    break;
    case 21:  // Weather obs report from ship
      // TODO(schwehr): untested - no messages found
      ais8_1_21_append_pydict(nmea_payload, dict, pad);
      break;
    case 22:  // Area notice
      ais8_1_22_append_pydict(nmea_payload, dict, pad);
      break;
      // 23 is ADDRESSED ONLY
    case 24:  // Extended ship static and voyage-related
      ais8_1_24_append_pydict(nmea_payload, dict, pad);
      break;
      // no 25
    case 26:  // Env Sensor Report
      ais8_1_26_append_pydict(nmea_payload, dict, pad);
      break;
    case 27:  // Route
      ais8_1_27_append_pydict(nmea_payload, dict, pad);
      break;
      // no 28
    case 29:  // IMO Circ 289 Text description
      ais8_1_29_append_pydict(nmea_payload, dict, pad);
      break;
    case 31:  // IMO Circ 289 Meteorological and Hydrographic data
      ais8_1_31_append_pydict(nmea_payload, dict, pad);
      break;

      // ITU 1371-1 only: 3.10 - IFM 40: Number of persons on board

    default:
      DictSafeSetItem(dict, "parsed", false);
      break;
    }
    break;
  case 200:  // River Information Systems ECE-TRANS-SC3-2006-10r-RIS.pdf
    switch (msg.fi) {
    case 10:  // Inland ship static and voyage related data
      ais8_200_10_append_pydict(nmea_payload, dict, pad);
      break;
      // 21: Addressed only
      // 22: Addressed only
    case 23:  // EMMA warning
      ais8_200_23_append_pydict(nmea_payload, dict, pad);
      break;
    case 24:  // Water level
      ais8_200_24_append_pydict(nmea_payload, dict, pad);
      break;
    case 40:  // A-to-N Signal status
      ais8_200_40_append_pydict(nmea_payload, dict, pad);
      break;
    case 55:  // Inland number of persons on board
      ais8_200_55_append_pydict(nmea_payload, dict, pad);
      break;
    default:
      DictSafeSetItem(dict, "parsed", false);
      break;
    }
    break;
    // case 366:  // United states
    // TODO(schwehr): implement
    //  break;
  default:
    DictSafeSetItem(dict, "parsed", false);
    // TODO(schwehr): raise exception or return standin?
  }
  // TODO(schwehr): check for error here?
  return dict;
}

PyObject*
ais9_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);
  Ais9 msg(nmea_payload, pad);
  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais9: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", 9);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);

  DictSafeSetItem(dict, "alt", msg.alt);
  DictSafeSetItem(dict, "sog", msg.sog);

  DictSafeSetItem(dict, "position_accuracy", msg.position_accuracy);
  DictSafeSetItem(dict, "x", msg.x);
  DictSafeSetItem(dict, "y", msg.y);

  DictSafeSetItem(dict, "cog", msg.cog);
  DictSafeSetItem(dict, "timestamp", msg.timestamp);
  DictSafeSetItem(dict, "alt_sensor", msg.alt_sensor);
  DictSafeSetItem(dict, "spare", msg.spare);
  DictSafeSetItem(dict, "dte", msg.dte);
  DictSafeSetItem(dict, "spare2", msg.spare2);
  DictSafeSetItem(dict, "raim", msg.raim);

  if (0 == msg.commstate_flag) {
    // SOTMDA
    DictSafeSetItem(dict, "slot_timeout", msg.slot_timeout);

    if (msg.received_stations_valid)
      DictSafeSetItem(dict, "received_stations", msg.received_stations);
    if (msg.slot_number_valid)
      DictSafeSetItem(dict, "slot_number", msg.slot_number);
    if (msg.utc_valid) {
      DictSafeSetItem(dict, "utc_hour", msg.utc_hour);
      DictSafeSetItem(dict, "utc_min", msg.utc_min);
      DictSafeSetItem(dict, "utc_spare", msg.utc_spare);
    }
    if (msg.slot_offset_valid)
      DictSafeSetItem(dict, "slot_offset", msg.slot_offset);
  } else {
    // ITDMA
    DictSafeSetItem(dict, "slot_increment", msg.slot_increment);
    DictSafeSetItem(dict, "slots_to_allocate", msg.slots_to_allocate);
    DictSafeSetItem(dict, "keep_flag", msg.keep_flag);
  }

  return dict;
}

// :
PyObject*
ais10_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);
  assert(pad < 6);

  Ais10 msg(nmea_payload, pad);
  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais10: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", 10);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);

  DictSafeSetItem(dict, "spare", msg.spare);
  DictSafeSetItem(dict, "dest_mmsi", msg.dest_mmsi);
  DictSafeSetItem(dict, "spare2", msg.spare2);

  return dict;
}

// msg 11 ';' - See msg 4

// 12 - '<'
PyObject*
ais12_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);
  Ais12 msg(nmea_payload, pad);
  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais9: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", 12);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);

  DictSafeSetItem(dict, "seq_num", msg.seq_num);
  DictSafeSetItem(dict, "dest_mmsi", msg.dest_mmsi);
  DictSafeSetItem(dict, "retransmitted", msg.retransmitted);

  DictSafeSetItem(dict, "spare", msg.spare);
  DictSafeSetItem(dict, "text", msg.text);

  return dict;
}


// msg 13 - See msg 7


// 14 - '>'
PyObject*
ais14_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);
  Ais14 msg(nmea_payload, pad);

  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais14: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", msg.message_id);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);
  DictSafeSetItem(dict, "spare", msg.spare);

  DictSafeSetItem(dict, "text", msg.text);

  return dict;
}

// '?'
PyObject*
ais15_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);
  Ais15 msg(nmea_payload, pad);

  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais15: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", msg.message_id);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);

  DictSafeSetItem(dict, "spare", msg.spare);
  DictSafeSetItem(dict, "mmsi_1", msg.mmsi_1);
  DictSafeSetItem(dict, "msg_1_1", msg.msg_1_1);
  DictSafeSetItem(dict, "slot_offset_1_1", msg.slot_offset_1_1);

  DictSafeSetItem(dict, "spare2", msg.spare2);
  DictSafeSetItem(dict, "dest_msg_1_2", msg.dest_msg_1_2);
  DictSafeSetItem(dict, "slot_offset_1_2", msg.slot_offset_1_2);

  DictSafeSetItem(dict, "spare3", msg.spare3);
  DictSafeSetItem(dict, "mmsi_2", msg.mmsi_2);
  DictSafeSetItem(dict, "msg_2", msg.msg_2);
  DictSafeSetItem(dict, "slot_offset_2", msg.slot_offset_2);
  DictSafeSetItem(dict, "spare4", msg.spare4);

  return dict;
}

// '@'
PyObject*
ais16_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);
  Ais16 msg(nmea_payload, pad);

  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais16: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", msg.message_id);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);

  DictSafeSetItem(dict, "spare", msg.spare);
  DictSafeSetItem(dict, "dest_mmsi_a", msg.dest_mmsi_a);
  DictSafeSetItem(dict, "offset_a", msg.offset_a);
  DictSafeSetItem(dict, "inc_a", msg.inc_a);

  if (-1 != msg.spare2) DictSafeSetItem(dict, "spare2", msg.spare2);
  if (-1 != msg.dest_mmsi_b) {
    DictSafeSetItem(dict, "dest_mmsi_b", msg.dest_mmsi_b);
    DictSafeSetItem(dict, "offset_b", msg.offset_b);
    DictSafeSetItem(dict, "inc_b", msg.inc_b);
  }

  return dict;
}

// 'A' - GNSS differential - TODO(schwehr): incomplete
PyObject*
ais17_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);
  Ais17 msg(nmea_payload, pad);

  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais17: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", msg.message_id);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);

  DictSafeSetItem(dict, "spare", msg.spare);
  DictSafeSetItem(dict, "x", msg.x);
  DictSafeSetItem(dict, "y", msg.y);
  DictSafeSetItem(dict, "spare2", msg.spare2);

  return dict;
}

// 'B'
PyObject*
ais18_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);
  Ais18 msg(nmea_payload, pad);
  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais18: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", msg.message_id);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);

  DictSafeSetItem(dict, "spare", msg.spare);
  DictSafeSetItem(dict, "sog", msg.sog);
  DictSafeSetItem(dict, "position_accuracy", msg.position_accuracy);
  DictSafeSetItem(dict, "x", msg.x);
  DictSafeSetItem(dict, "y", msg.y);
  DictSafeSetItem(dict, "cog", msg.cog);
  DictSafeSetItem(dict, "true_heading", msg.true_heading);
  DictSafeSetItem(dict, "timestamp", msg.timestamp);

  DictSafeSetItem(dict, "spare2", msg.spare2);

  DictSafeSetItem(dict, "unit_flag", msg.unit_flag);
  DictSafeSetItem(dict, "display_flag", msg.display_flag);
  DictSafeSetItem(dict, "dsc_flag", msg.dsc_flag);
  DictSafeSetItem(dict, "band_flag", msg.band_flag);
  DictSafeSetItem(dict, "m22_flag", msg.m22_flag);
  DictSafeSetItem(dict, "mode_flag", msg.mode_flag);

  DictSafeSetItem(dict, "raim", msg.raim);

  DictSafeSetItem(dict, "commstate_flag", msg.commstate_flag);
  if (0 == msg.unit_flag) {
    if (0 == msg.commstate_flag) {
      // SOTDMA
      DictSafeSetItem(dict, "slot_timeout", msg.slot_timeout);
      switch (msg.slot_timeout) {
      case 0:
        DictSafeSetItem(dict, "slot_offset", msg.slot_offset);
        break;
      case 1:
        DictSafeSetItem(dict, "utc_hour", msg.utc_hour);
        DictSafeSetItem(dict, "utc_min", msg.utc_min);
        DictSafeSetItem(dict, "utc_spare", msg.utc_spare);
        break;
      case 2:  // FALLTHROUGH
      case 4:  // FALLTHROUGH
      case 6:
        DictSafeSetItem(dict, "slot_number", msg.slot_number);
        break;
      case 3:  // FALLTHROUGH
      case 5:  // FALLTHROUGH
      case 7:
        DictSafeSetItem(dict, "received_stations", msg.received_stations);
        break;
      default:
        assert(false);  // Should never get here.
      }

    } else {
      // ITDMA
      DictSafeSetItem(dict, "slot_increment", msg.slot_increment);
      DictSafeSetItem(dict, "slots_to_allocate", msg.slots_to_allocate);
      DictSafeSetItem(dict, "keep_flag", msg.keep_flag);
    }
  }  // do nothing if unit flag is 1... in CS mode and no commstate

  return dict;
}

// 'C'
PyObject*
ais19_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);
  Ais19 msg(nmea_payload, pad);
  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais19: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", msg.message_id);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);

  DictSafeSetItem(dict, "spare", msg.spare);
  DictSafeSetItem(dict, "sog", msg.sog);
  DictSafeSetItem(dict, "position_accuracy", msg.position_accuracy);
  DictSafeSetItem(dict, "x", msg.x);
  DictSafeSetItem(dict, "y", msg.y);
  DictSafeSetItem(dict, "cog", msg.cog);
  DictSafeSetItem(dict, "true_heading", msg.true_heading);
  DictSafeSetItem(dict, "timestamp", msg.timestamp);

  DictSafeSetItem(dict, "spare2", msg.spare2);

  DictSafeSetItem(dict, "name", msg.name);
  DictSafeSetItem(dict, "type_and_cargo", msg.type_and_cargo);
  DictSafeSetItem(dict, "dim_a", msg.dim_a);
  DictSafeSetItem(dict, "dim_b", msg.dim_b);
  DictSafeSetItem(dict, "dim_c", msg.dim_c);
  DictSafeSetItem(dict, "dim_d", msg.dim_d);
  DictSafeSetItem(dict, "fix_type", msg.fix_type);

  DictSafeSetItem(dict, "raim", msg.raim);

  DictSafeSetItem(dict, "dte", msg.dte);
  DictSafeSetItem(dict, "assigned_mode", msg.assigned_mode);
  DictSafeSetItem(dict, "spare3", msg.spare3);

  return dict;
}

// TODO(schwehr): 'D' - data link management
PyObject*
ais20_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);
  Ais20 msg(nmea_payload, pad);
  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais20: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", msg.message_id);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);
  DictSafeSetItem(dict, "spare", msg.spare);

  int list_size = 1;
  if (msg.group_valid_4) list_size = 4;
  else if (msg.group_valid_3) list_size = 3;
  else if (msg.group_valid_2) list_size = 2;

  PyObject *list = PyList_New(list_size);

  {
    PyObject *reservation = PyDict_New();
    DictSafeSetItem(reservation, "offset", msg.offset_1);
    DictSafeSetItem(reservation, "num_slots", msg.num_slots_1);
    DictSafeSetItem(reservation, "timeout", msg.timeout_1);
    DictSafeSetItem(reservation, "incr", msg.incr_1);
    PyList_SetItem(list, 0, reservation);
  }

  if (msg.group_valid_2) {
    PyObject *reservation = PyDict_New();
    DictSafeSetItem(reservation, "offset", msg.offset_2);
    DictSafeSetItem(reservation, "num_slots", msg.num_slots_2);
    DictSafeSetItem(reservation, "timeout", msg.timeout_2);
    DictSafeSetItem(reservation, "incr", msg.incr_2);
    PyList_SetItem(list, 1, reservation);
  }

  if (msg.group_valid_3) {
    PyObject *reservation = PyDict_New();
    DictSafeSetItem(reservation, "offset", msg.offset_3);
    DictSafeSetItem(reservation, "num_slots", msg.num_slots_3);
    DictSafeSetItem(reservation, "timeout", msg.timeout_3);
    DictSafeSetItem(reservation, "incr", msg.incr_3);
    PyList_SetItem(list, 2, reservation);
  }

  if (msg.group_valid_4) {
    PyObject *reservation = PyDict_New();
    DictSafeSetItem(reservation, "offset", msg.offset_4);
    DictSafeSetItem(reservation, "num_slots", msg.num_slots_4);
    DictSafeSetItem(reservation, "timeout", msg.timeout_4);
    DictSafeSetItem(reservation, "incr", msg.incr_4);
    PyList_SetItem(list, 3, reservation);
  }

  PyDict_SetItem(dict, PyUnicode_FromString("reservations"), list);

  return dict;
}


// E - ATON Aid to Navigation
PyObject*
ais21_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);
  Ais21 msg(nmea_payload, pad);
  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais21: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", msg.message_id);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);
  DictSafeSetItem(dict, "spare", msg.spare);

  DictSafeSetItem(dict, "aton_type", msg.aton_type);
  DictSafeSetItem(dict, "name", msg.name);
  DictSafeSetItem(dict, "position_accuracy", msg.position_accuracy);
  DictSafeSetItem(dict, "x", msg.x);
  DictSafeSetItem(dict, "y", msg.y);
  DictSafeSetItem(dict, "dim_a", msg.dim_a);
  DictSafeSetItem(dict, "dim_b", msg.dim_b);
  DictSafeSetItem(dict, "dim_c", msg.dim_c);
  DictSafeSetItem(dict, "dim_d", msg.dim_d);
  DictSafeSetItem(dict, "fix_type", msg.fix_type);
  DictSafeSetItem(dict, "timestamp", msg.timestamp);
  DictSafeSetItem(dict, "off_pos", msg.off_pos);
  DictSafeSetItem(dict, "aton_status", msg.aton_status);
  DictSafeSetItem(dict, "raim", msg.raim);
  DictSafeSetItem(dict, "virtual_aton", msg.virtual_aton);
  DictSafeSetItem(dict, "assigned_mode", msg.assigned_mode);

  return dict;
}

// F - channel mangement
PyObject*
ais22_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);
  Ais22 msg(nmea_payload, pad);
  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais22: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", msg.message_id);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);
  DictSafeSetItem(dict, "spare", msg.spare);

  DictSafeSetItem(dict, "chan_a", msg.chan_a);
  DictSafeSetItem(dict, "chan_b", msg.chan_b);
  DictSafeSetItem(dict, "txrx_mode", msg.txrx_mode);
  DictSafeSetItem(dict, "power_low", msg.power_low);

  if (msg.pos_valid) {
    DictSafeSetItem(dict, "x1", msg.x1);
    DictSafeSetItem(dict, "y1", msg.y1);
    DictSafeSetItem(dict, "x2", msg.x2);
    DictSafeSetItem(dict, "y2", msg.y2);
  } else {
    DictSafeSetItem(dict, "dest_mmsi_1", msg.dest_mmsi_1);
    DictSafeSetItem(dict, "dest_mmsi_2", msg.dest_mmsi_2);
  }

  DictSafeSetItem(dict, "chan_a_bandwidth", msg.chan_a_bandwidth);
  DictSafeSetItem(dict, "chan_b_bandwidth", msg.chan_b_bandwidth);
  DictSafeSetItem(dict, "zone_size", msg.zone_size);
  DictSafeSetItem(dict, "spare2", msg.spare2);

  return dict;
}


// F - group assignment command
PyObject*
ais23_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);
  Ais23 msg(nmea_payload, pad);
  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais23: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", msg.message_id);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);
  DictSafeSetItem(dict, "spare", msg.spare);

  DictSafeSetItem(dict, "x1", msg.x1);
  DictSafeSetItem(dict, "y1", msg.y1);
  DictSafeSetItem(dict, "x2", msg.x2);
  DictSafeSetItem(dict, "y2", msg.y2);

  DictSafeSetItem(dict, "station_type", msg.station_type);
  DictSafeSetItem(dict, "type_and_cargo", msg.type_and_cargo);
  DictSafeSetItem(dict, "spare2", msg.spare2);

  DictSafeSetItem(dict, "txrx_mode", msg.txrx_mode);
  DictSafeSetItem(dict, "interval_raw", msg.interval_raw);
  DictSafeSetItem(dict, "quiet", msg.quiet);
  DictSafeSetItem(dict, "spare3", msg.spare3);

  return dict;
}

// H - Static data report
PyObject*
ais24_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);
  Ais24 msg(nmea_payload, pad);
  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais24: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", msg.message_id);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);

  DictSafeSetItem(dict, "part_num", msg.part_num);

  switch (msg.part_num) {
  case 0:  // Part A
    DictSafeSetItem(dict, "name", msg.name);
    break;
  case 1:  // Part B
    DictSafeSetItem(dict, "type_and_cargo", msg.type_and_cargo);
    DictSafeSetItem(dict, "vendor_id", msg.vendor_id);
    DictSafeSetItem(dict, "callsign", msg.callsign);
    DictSafeSetItem(dict, "dim_a", msg.dim_a);
    DictSafeSetItem(dict, "dim_b", msg.dim_b);
    DictSafeSetItem(dict, "dim_c", msg.dim_c);
    DictSafeSetItem(dict, "dim_d", msg.dim_d);
    DictSafeSetItem(dict, "spare", msg.spare);
    break;
  case 2:  // FALLTHROUGH - not yet defined by ITU
  case 3:  // FALLTHROUGH - not yet defined by ITU
  default:
    // status = AIS_ERR_BAD_MSG_CONTENT;
    // TODO(schwehr): setup python exception
    return NULL;
  }

  return dict;
}

// I - single slot binary message
PyObject*
ais25_to_pydict(const char *nmea_payload, const size_t pad) {
  assert(nmea_payload);
  Ais25 msg(nmea_payload, pad);
  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais25: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", msg.message_id);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);

  // TODO(schwehr) use_app_id
  if (msg.dest_mmsi_valid) DictSafeSetItem(dict, "dest_mmsi", msg.dest_mmsi);
  if (msg.use_app_id) {
    DictSafeSetItem(dict, "dac", msg.dac);
    DictSafeSetItem(dict, "fi", msg.fi);
  }

  // TODO(schwehr): handle payload

  return dict;
}

// J - multi-slot binary message with commstate
PyObject*
ais26_to_pydict(const char *nmea_payload, const size_t pad) {
  Ais26 msg(nmea_payload, pad);
  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais26: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", msg.message_id);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);

  if (msg.dest_mmsi_valid) DictSafeSetItem(dict, "dest_mmsi", msg.dest_mmsi);
  if (msg.use_app_id) {
    DictSafeSetItem(dict, "dac", msg.dac);
    DictSafeSetItem(dict, "fi", msg.fi);
  }

  // TODO(schwehr): handle payload

  DictSafeSetItem(dict, "sync_state", msg.sync_state);
  if (0 == msg.commstate_flag) {
    // SOTDMA
    if (msg.received_stations_valid)
      DictSafeSetItem(dict, "received_stations", msg.received_stations);
    if (msg.slot_number_valid)
      DictSafeSetItem(dict, "slot_number", msg.slot_number);
    if (msg.utc_valid) {
      DictSafeSetItem(dict, "utc_hour", msg.utc_hour);
      DictSafeSetItem(dict, "utc_min", msg.utc_min);
      DictSafeSetItem(dict, "utc_spare", msg.utc_spare);
    }
    if (msg.slot_offset_valid)
      DictSafeSetItem(dict, "slot_offset", msg.slot_offset);
  } else {
    // ITDMA
    DictSafeSetItem(dict, "slot_increment", msg.slot_increment);
    DictSafeSetItem(dict, "slots_to_allocate", msg.slots_to_allocate);
    DictSafeSetItem(dict, "keep_flag", msg.keep_flag);
  }
  return dict;
}


// J - multi-slot binary message with commstate
PyObject*
ais27_to_pydict(const char *nmea_payload, const size_t pad) {
  Ais27 msg(nmea_payload, pad);
  if (msg.had_error()) {
    PyErr_Format(ais_py_exception, "Ais27: %s",
                 AIS_STATUS_STRINGS[msg.get_error()]);
    return NULL;
  }

  PyObject *dict = PyDict_New();
  DictSafeSetItem(dict, "id", msg.message_id);
  DictSafeSetItem(dict, "repeat_indicator", msg.repeat_indicator);
  DictSafeSetItem(dict, "mmsi", msg.mmsi);

  DictSafeSetItem(dict, "position_accuracy", msg.position_accuracy);
  DictSafeSetItem(dict, "raim", msg.raim);
  DictSafeSetItem(dict, "nav_status", msg.nav_status);
  DictSafeSetItem(dict, "x", msg.x);
  DictSafeSetItem(dict, "y", msg.y);
  DictSafeSetItem(dict, "sog", msg.sog);
  DictSafeSetItem(dict, "cog", msg.cog);
  DictSafeSetItem(dict, "gnss", msg.gnss);
  DictSafeSetItem(dict, "spare", msg.spare);
  return dict;
}


static PyObject *
decode(PyObject *self, PyObject *args) {
  int _pad;
  const char *nmea_payload;
  // TODO(schwehr): what to do about if no pad bits?  Maybe warn and set to 0?
  if (!PyArg_ParseTuple(args, "si", &nmea_payload, &_pad)) {
    _pad = 0;
    if (!PyArg_ParseTuple(args, "s", &nmea_payload)) {
      PyErr_Format(ais_py_exception, "ais.decode: expected (str, int)");
      return NULL;
    }
  }
  const size_t pad = _pad;
  PyObject *result = 0;

  // The grand dispatcher
  switch (nmea_payload[0]) {
    // Class A Position
  case '1':  // FALLTHROUGH
  case '2':  // FALLTHROUGH
  case '3':
    result = ais1_2_3_to_pydict(nmea_payload, pad);
    break;

  case '4':  // FALLTHROUGH - 4 - Basestation report
  case ';':  //  11 - UTC date response
    result = ais4_11_to_pydict(nmea_payload, pad);
    break;

  case '5':  // 5 - Ship and Cargo
    result = ais5_to_pydict(nmea_payload, pad);
    break;

  case '6':  // 6 - Addressed binary message
    result = ais6_to_pydict(nmea_payload, pad);
    break;

  case '7':  // FALLTHROUGH - 7 - ACK for addressed binary message
  case '=':  // 13 - ASRM Ack  (safety message)
    result = ais7_13_to_pydict(nmea_payload, pad);
    break;

  case '8':  // 8 - Binary broadcast message (BBM)
    result = ais8_to_pydict(nmea_payload, pad);
    break;

  case '9':  // 9 - SAR Position
    result = ais9_to_pydict(nmea_payload, pad);
    break;

  case ':':  // 10 - UTC Query
    result = ais10_to_pydict(nmea_payload, pad);
    break;

    // ':' 11 - See 4

  case '<':  // 12 - ASRM
    result = ais12_to_pydict(nmea_payload, pad);
    break;

    // 13 - See 7

  case '>':  // 14 - SRBM - Safety broadcast
    result = ais14_to_pydict(nmea_payload, pad);
    break;

  case '?':  // 15 - Interrogation
    result = ais15_to_pydict(nmea_payload, pad);
    break;

  case '@':  // 16 - Assigned mode command
    result = ais16_to_pydict(nmea_payload, pad);
    break;

  case 'A':  // 17 - GNSS broadcast
    result = ais17_to_pydict(nmea_payload, pad);
    break;

  case 'B':  // 18 - Position, Class B
    result = ais18_to_pydict(nmea_payload, pad);
    break;

  case 'C':  // 19 - Position and ship, Class B
    result = ais19_to_pydict(nmea_payload, pad);
    break;

  case 'D':  // 20 - Data link management
    result = ais20_to_pydict(nmea_payload, pad);
    break;

  case 'E':  // 21 - Aids to navigation report
    result = ais21_to_pydict(nmea_payload, pad);
    break;

  case 'F':  // 22 - Channel Management
    result = ais22_to_pydict(nmea_payload, pad);
    break;

  case 'G':  // 23 - Group Assignment Command
    result = ais23_to_pydict(nmea_payload, pad);
    break;

  case 'H':  // 24 - Static data report
    result = ais24_to_pydict(nmea_payload, pad);
    break;

  case 'I':  // 25 - Single slot binary message - addressed or broadcast
    // TODO(schwehr): handle payloads
    result = ais25_to_pydict(nmea_payload, pad);
    break;

  case 'J':  // 26 - Multi slot binary message with comm state
    result = ais26_to_pydict(nmea_payload, pad);  // TODO(schwehr): payloads
    break;

  case 'K':  // 27 - Long-range AIS broadcast message
    result = ais27_to_pydict(nmea_payload, pad);
    break;

  case 'L':  // 28 - UNKNOWN
    PyErr_Format(ais_py_exception, "ais.decode: message 28 (L) not handled");
    break;

  default:
    PyErr_Format(ais_py_exception, "ais.decode: unknown message - %c",
                 nmea_payload[0]);
  }

  return result;
}

static PyMethodDef ais_methods[] = {
  {"decode", decode, METH_VARARGS, "Return a dictionary for a NMEA string"},
  {NULL, NULL, 0, NULL},
};

#if 0
  PyMODINIT_FUNC
  initais(void) {
    PyObject *mod;
    mod = Py_InitModule("ais", ais_methods);
    if (mod == NULL) {
      cerr << "UNABLE TO LOAD MODULE";
      return;
    }
    BuildNmeaLookup();
  }
#else
  //////////////////////////////////////////////////////////////////////
  // Python 3 initialization
  //////////////////////////////////////////////////////////////////////

struct module_state {
  PyObject *error;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

#if 0
static PyObject *
error_out(PyObject *m) {
  struct module_state *st = GETSTATE(m);
  PyErr_SetString(st->error, "something bad happened");
  return NULL;
}

static PyMethodDef ais_methods[] = {
  {"error_out", (PyCFunction)error_out, METH_NOARGS, NULL},
  {NULL, NULL}
};
#endif

#if PY_MAJOR_VERSION >= 3

static int ais_traverse(PyObject *m, visitproc visit, void *arg) {
  Py_VISIT(GETSTATE(m)->error);
  return NULL;
}

static int ais_clear(PyObject *m) {
  Py_CLEAR(GETSTATE(m)->error);
  return NULL;
}

static struct PyModuleDef moduledef = {
  PyModuleDef_HEAD_INIT,
  "ais",
  NULL,
  sizeof(struct module_state),
  ais_methods,
  NULL,
  ais_traverse,
  ais_clear,
  NULL
};

#define INITERROR return NULL

PyObject *
PyInit_ais(void)

#warning "Should be okay"

#else

#define INITERROR return

void
initais(void)
#endif
  {
#if PY_MAJOR_VERSION >= 3
PyObject *module = PyModule_Create(&moduledef);
#else
PyObject *module = Py_InitModule("ais", ais_methods);
#endif

  if (module == NULL)
    INITERROR;
  struct module_state *st = GETSTATE(module);

  st->error = PyErr_NewException(const_cast<char *>(exception_name.c_str()),
                                 NULL, NULL);
  if (st->error == NULL) {
    Py_DECREF(module);
    INITERROR;
  }

  // Initialize the lookuptable and exception
  BuildNmeaLookup();
  ais_py_exception = PyErr_NewException(const_cast<char *>("ais.decode.error"),
                                        NULL, NULL);

#if PY_MAJOR_VERSION >= 3
  return module;
#endif
}

#endif
}  // extern "C"
