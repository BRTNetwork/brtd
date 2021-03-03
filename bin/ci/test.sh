#!/usr/bin/env bash

set -o xtrace
set -o errexit

# Set to 'true' to run the known "manual" tests in brtd.
MANUAL_TESTS=${MANUAL_TESTS:-false}
# The maximum number of concurrent tests.
CONCURRENT_TESTS=${CONCURRENT_TESTS:-$(nproc)}
# The path to brtd.
brtd=${brtd:-build/brtd}
# Additional arguments to brtd.
brtd_ARGS=${brtd_ARGS:-}

function join_by { local IFS="$1"; shift; echo "$*"; }

declare -a manual_tests=(
  'beast.chrono.abstract_clock'
  'beast.unit_test.print'
  'ripple.NodeStore.Timing'
  'ripple.app.Flow_manual'
  'ripple.app.NoRippleCheckLimits'
  'ripple.app.PayStrandAllPairs'
  'ripple.consensus.ByzantineFailureSim'
  'ripple.consensus.DistributedValidators'
  'ripple.consensus.ScaleFreeSim'
  'ripple.tx.CrossingLimits'
  'ripple.tx.FindOversizeCross'
  'ripple.tx.Offer_manual'
  'ripple.tx.OversizeMeta'
  'ripple.tx.PlumpBook'
)

if [[ ${MANUAL_TESTS} == 'true' ]]; then
  brtd_ARGS+=" --unittest=$(join_by , "${manual_tests[@]}")"
else
  brtd_ARGS+=" --unittest --quiet --unittest-log"
fi
brtd_ARGS+=" --unittest-jobs ${CONCURRENT_TESTS}"

${brtd} ${brtd_ARGS}
