#!/bin/sh
# Copyright 2005-2015 ECMWF.
#
# This software is licensed under the terms of the Apache Licence Version 2.0
# which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
#
# In applying this licence, ECMWF does not waive the privileges and immunities granted to it by
# virtue of its status as an intergovernmental organisation nor does it submit to any jurisdiction.
#

. ./include.sh

REDIRECT=/dev/null

# First check if Adaptive Entropy Coding (AEC) feature is enabled
skip_test=0
src_config=${src_dir}/config.h
if [ -f ${src_config} ]; then
  set +e
  grep '#undef HAVE_LIBAEC' ${src_config} >/dev/null
  status=$?
  set -e
  if [ $status -eq 0 ]; then
    # Found the string so feature is disabled
    skip_test=1
  fi
fi
if [ $skip_test -eq 1 ]; then
  #echo "AEC feature was not enabled. Skipping this test."
  exit 0
fi
echo "AEC feature was enabled."

BLACKLIST="totalLength,section5Length,section7Length,dataRepresentationTemplateNumber,typeOfPacking"

infile=${data_dir}/ccsds.grib2

rm -f $infile.1 $infile.2 || true
${tools_dir}grib_set -f -r -s packingType=grid_simple $infile $infile.1 2> $REDIRECT
${tools_dir}grib_compare -P -b $BLACKLIST,typeOfCompressionUsed,targetCompressionRatio $infile $infile.1 > $REDIRECT
${tools_dir}grib_set -r -s packingType=grid_ccsds $infile.1 $infile.2
${tools_dir}grib_compare -P -b $BLACKLIST $infile.1 $infile.2 > $REDIRECT

templateNumber=`${tools_dir}grib_get -p dataRepresentationTemplateNumber $infile.2`

if [ $templateNumber -ne 42 ]
then
  echo dataRepresentationTemplateNumber=$templateNumber
  exit 1
fi

rm -f $infile.1 $infile.2 || true

infile=${data_dir}/reduced_latlon_surface.grib2

${tools_dir}grib_set -r -s packingType=grid_ccsds $infile $infile.1
${tools_dir}grib_compare -P -b $BLACKLIST $infile $infile.1 > $REDIRECT
${tools_dir}grib_set -f -r -s packingType=grid_simple $infile.1 $infile.2 2> $REDIRECT
${tools_dir}grib_compare -P -b $BLACKLIST,typeOfCompressionUsed,targetCompressionRatio $infile.1 $infile.2  > $REDIRECT

res1=`${tools_dir}grib_get '-F%1.2f' -p min,max,avg $infile`
res2=`${tools_dir}grib_get '-F%1.2f' -p min,max,avg $infile.1`
res3=`${tools_dir}grib_get '-F%1.2f' -p min,max,avg $infile.2`
[ "$res1" = "$res2" ]
[ "$res1" = "$res3" ]

rm -f $infile.1 $infile.2 || true
