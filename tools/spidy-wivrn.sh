#!/usr/bin/env bash
# Start Spidy VR with WiVRn; see spidy-linux.sh for the steps.
SPIDY_RUNTIME=wivrn exec "$(dirname "${BASH_SOURCE[0]}")/spidy-linux.sh" "$@"
