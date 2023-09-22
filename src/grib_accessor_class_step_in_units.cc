/*
 * (C) Copyright 2005- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 *
 * In applying this licence, ECMWF does not waive the privileges and immunities granted to it by
 * virtue of its status as an intergovernmental organisation nor does it submit to any jurisdiction.
 */

#include "grib_api_internal.h"
#include "step.h"
#include "step_utilities.h"
#include <stdexcept>
/*
  This is used by make_class.pl

  START_CLASS_DEF
  CLASS      = accessor
  SUPER      = grib_accessor_class_long
  IMPLEMENTS = unpack_long;pack_long
  IMPLEMENTS = unpack_double
  IMPLEMENTS = unpack_string;pack_string
  IMPLEMENTS = init;dump
  MEMBERS = const char* forecast_time_value
  MEMBERS = const char* forecast_time_unit
  MEMBERS = const char* step_units
  MEMBERS = const char* time_range_unit 
  MEMBERS = const char* time_range_value

  END_CLASS_DEF

 */

/* START_CLASS_IMP */

/*

Don't edit anything between START_CLASS_IMP and END_CLASS_IMP
Instead edit values between START_CLASS_DEF and END_CLASS_DEF
or edit "accessor.class" and rerun ./make_class.pl

*/

static int pack_long(grib_accessor*, const long* val, size_t* len);
static int pack_string(grib_accessor*, const char*, size_t* len);
static int unpack_long(grib_accessor*, long* val, size_t* len);
static int unpack_double(grib_accessor*, double* val, size_t* len);
static int unpack_string(grib_accessor*, char*, size_t* len);
static void dump(grib_accessor*, grib_dumper*);
static void init(grib_accessor*, const long, grib_arguments*);

typedef struct grib_accessor_step_in_units
{
    grib_accessor att;
    /* Members defined in gen */
    /* Members defined in long */
    /* Members defined in step_in_units */
    const char* forecast_time_value;
    const char* forecast_time_unit;
    const char* step_units;
    const char* time_range_unit;
    const char* time_range_value;
} grib_accessor_step_in_units;

extern grib_accessor_class* grib_accessor_class_long;

static grib_accessor_class _grib_accessor_class_step_in_units = {
    &grib_accessor_class_long,                      /* super */
    "step_in_units",                      /* name */
    sizeof(grib_accessor_step_in_units),  /* size */
    0,                           /* inited */
    0,                           /* init_class */
    &init,                       /* init */
    0,                  /* post_init */
    0,                    /* destroy */
    &dump,                       /* dump */
    0,                /* next_offset */
    0,              /* get length of string */
    0,                /* get number of values */
    0,                 /* get number of bytes */
    0,                /* get offset to bytes */
    0,            /* get native type */
    0,                /* get sub_section */
    0,               /* pack_missing */
    0,                 /* is_missing */
    &pack_long,                  /* pack_long */
    &unpack_long,                /* unpack_long */
    0,                /* pack_double */
    0,                 /* pack_float */
    &unpack_double,              /* unpack_double */
    0,               /* unpack_float */
    &pack_string,                /* pack_string */
    &unpack_string,              /* unpack_string */
    0,          /* pack_string_array */
    0,        /* unpack_string_array */
    0,                 /* pack_bytes */
    0,               /* unpack_bytes */
    0,            /* pack_expression */
    0,              /* notify_change */
    0,                /* update_size */
    0,             /* preferred_size */
    0,                     /* resize */
    0,      /* nearest_smaller_value */
    0,                       /* next accessor */
    0,                    /* compare vs. another accessor */
    0,      /* unpack only ith value (double) */
    0,       /* unpack only ith value (float) */
    0,  /* unpack a given set of elements (double) */
    0,   /* unpack a given set of elements (float) */
    0,     /* unpack a subarray */
    0,                      /* clear */
    0,                 /* clone accessor */
};


grib_accessor_class* grib_accessor_class_step_in_units = &_grib_accessor_class_step_in_units;

/* END_CLASS_IMP */

static void init(grib_accessor* a, const long l, grib_arguments* c)
{
    grib_accessor_step_in_units* self = (grib_accessor_step_in_units*)a;
    int n                             = 0;

    self->forecast_time_value = grib_arguments_get_name(grib_handle_of_accessor(a), c, n++);
    self->forecast_time_unit  = grib_arguments_get_name(grib_handle_of_accessor(a), c, n++);
    self->step_units          = grib_arguments_get_name(grib_handle_of_accessor(a), c, n++);
    self->time_range_unit     = grib_arguments_get_name(grib_handle_of_accessor(a), c, n++);
    self->time_range_value    = grib_arguments_get_name(grib_handle_of_accessor(a), c, n++);
}

static void dump(grib_accessor* a, grib_dumper* dumper)
{
    grib_dump_double(dumper, a, NULL);
}


static int unpack_long(grib_accessor* a, long* val, size_t* len)
{
    grib_accessor_step_in_units* self = (grib_accessor_step_in_units*)a;
    int err                           = 0;
    long forecast_time_value, forecast_time_unit, step_units;
    grib_handle* h = grib_handle_of_accessor(a);

    if ((err= grib_get_long_internal(h, "stepUnits", &step_units)) != GRIB_SUCCESS)
        return err;
    if ((err = grib_get_long_internal(h, self->forecast_time_unit, &forecast_time_unit)))
        return err;
    if ((err = grib_get_long_internal(h, self->forecast_time_value, &forecast_time_value)))
        return err;

    Step step{forecast_time_value, forecast_time_unit};

    if ((err = grib_set_long_internal(h, "startStepUnit", Unit{step_units}.value<long>())) != GRIB_SUCCESS)
        return err;

    *val = step.value<long>(Unit{step_units});

    return GRIB_SUCCESS;
}



static int unpack_double(grib_accessor* a, double * val, size_t* len)
{
    grib_accessor_step_in_units* self = (grib_accessor_step_in_units*)a;
    int err                           = 0;
    long forecast_time_value, forecast_time_unit, step_units;
    grib_handle* h = grib_handle_of_accessor(a);

    if ((err= grib_get_long_internal(h, "stepUnits", &step_units)) != GRIB_SUCCESS)
        return err;
    if ((err = grib_get_long_internal(h, self->forecast_time_unit, &forecast_time_unit)))
        return err;
    if ((err = grib_get_long_internal(h, self->forecast_time_value, &forecast_time_value)))
        return err;

    Step step{forecast_time_value, forecast_time_unit};

    if ((err = grib_set_long_internal(h, "startStepUnit", Unit{step_units}.value<long>())) != GRIB_SUCCESS)
        return err;

    *val = step.value<double>(Unit{step_units});

    return GRIB_SUCCESS;
}


int pack_long_new_(grib_accessor* a, const long start_step_value, const long start_step_unit) {
    grib_accessor_step_in_units* self = (grib_accessor_step_in_units*)a;
    grib_handle* h                    = grib_handle_of_accessor(a);
    int err                           = 0;
    long forecast_time_unit;
    long start_step_value_old;
    long start_step_unit_old;
    size_t len = 0;

    if ((err = grib_get_long_internal(h, self->forecast_time_unit, &forecast_time_unit)) != GRIB_SUCCESS)
        return err;
    if ((err = unpack_long(a, &start_step_value_old, &len)) != GRIB_SUCCESS)
        return err;
    if ((err = grib_get_long_internal(h, "startStepUnit", &start_step_unit_old)) != GRIB_SUCCESS)
        return err;

    Step start_step_old(start_step_value_old, start_step_unit_old);
    Step forecast_time(start_step_value, start_step_unit);
    Step time_range_new{};

    auto time_range_opt = get_step(h, self->time_range_value, self->time_range_unit);

    if (time_range_opt) {
        auto time_range = time_range_opt.value();
        time_range = time_range - (forecast_time - start_step_old);
        if (time_range.value<long>() < 0)
            time_range = Step{0l, time_range.unit()};
        auto [sa, sb] = find_common_units(forecast_time.optimize_unit(), time_range.optimize_unit());
        if ((err = set_step(h, self->forecast_time_value, self->forecast_time_unit, sa)) != GRIB_SUCCESS)
            return err;
        if ((err = grib_set_long_internal(h, "startStepUnit", forecast_time.unit().value<long>())) != GRIB_SUCCESS)
            return err;
        if ((err = set_step(h, self->time_range_value, self->time_range_unit, sb)) != GRIB_SUCCESS)
            return err;
        return GRIB_SUCCESS;
    }

    forecast_time.optimize_unit();
    if ((err = grib_set_long_internal(h, "startStepUnit", forecast_time.unit().value<long>())) != GRIB_SUCCESS)
        return err;
    if ((err = set_step(h, self->forecast_time_value, self->forecast_time_unit, forecast_time)) != GRIB_SUCCESS)
        return err;

    return GRIB_SUCCESS;
}


static int pack_long(grib_accessor* a, const long* val, size_t* len)
{
    grib_handle* h                   = grib_handle_of_accessor(a);
    int ret;

    long force_step_units;
    if ((ret = grib_get_long_internal(h, "forceStepUnits", &force_step_units)) != GRIB_SUCCESS)
        return ret;

    long start_step_unit;
    if (force_step_units == 255) {
        if ((ret = grib_get_long_internal(h, "startStepUnit", &start_step_unit)) != GRIB_SUCCESS)
            return ret;

        if (start_step_unit == 255)
            start_step_unit = Unit{Unit::Value::HOUR}.value<long>();
    }
    else {
        start_step_unit = force_step_units;
    }

    ret = pack_long_new_(a, *val, start_step_unit);

    return ret;
}


static int pack_string(grib_accessor* a, const char* val, size_t* len)
{
    int ret = 0;
    Step step = step_from_string(val);

    if ((ret = pack_long_new_(a, step.value<long>(), step.unit().value<long>())) != GRIB_SUCCESS)
        return ret;

    return GRIB_SUCCESS;
}

static int unpack_string(grib_accessor* a, char* val, size_t* len) 
{
    grib_accessor_step_in_units* self = (grib_accessor_step_in_units*)a;
    grib_handle* h                   = grib_handle_of_accessor(a);
    int ret = 0;
    long start_step_value;
    long start_step_unit;
    long step_units;
    size_t fp_format_len = 128;
    char fp_format[128];

    if ((ret = grib_get_long_internal(h, "startStep", &start_step_value)) != GRIB_SUCCESS)
        return ret;
    if ((ret = grib_get_long_internal(h, "startStepUnit", &start_step_unit)) != GRIB_SUCCESS)
        return ret;
    if ((ret = grib_get_long_internal(h, self->step_units, &step_units)))
        return ret;
    if ((ret = grib_get_string_internal(h, "format", fp_format, &fp_format_len)) != GRIB_SUCCESS)
        return ret;

    Step step{start_step_value, start_step_unit};
    std::stringstream ss;

    ss << step.value<std::string>(fp_format);

    size_t size = ss.str().size() + 1;

    if (*len < size)
        return GRIB_ARRAY_TOO_SMALL;

    *len = size;

    memcpy(val, ss.str().c_str(), size);

    return GRIB_SUCCESS;
}
