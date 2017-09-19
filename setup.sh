#!/bin/bash

echo "This script initializes your development environment"
echo "make sure that you have edited it and configured the correct paths first."

read

# ------------------- INSERT CORRET PATHS HERE --------------------------

# ---- tools ----
JLINK_COMMANDER_PATH="${HOME}/JLink_Linux_V498e_x86_64"

# ---- SDK stuff ----
INSTALL_PATH="${PWD}"

echo "export FRUITMESH_ROOT=${INSTALL_PATH}" >> ~/.bashrc

EHAL_PATH="${INSTALL_PATH}/sdk/ehal_2015_09_08"
CMSIS_PATH="${NSTALL_PATH}/sdk/arm_cmsis_4_3"
NRF51_SDK_PATH="${INSTALL_PATH}/sdk/nrf_sdk_9_0"
NRF52_SDK_PATH="${INSTALL_PATH}/sdk/nrf52_sdk_0_9_1"

# ------------------- DO NOT EDIT BELOW THIS LINE --------------------------


# -----------------------------------------------------------
echo "Creating links for tools"

cd tools

rm -f jlink

ln -s ${JLINK_COMMANDER_PATH} jlink


# -----------------------------------------------------------
# Links for SDK

cd ${EHAL_PATH}/ARM


rm -f CMSIS
ln -s ${CMSIS_PATH} CMSIS


cd ${EHAL_PATH}/ARM/Nordic/nRF51
rm -f nrf51_sdk
ln -s ${NRF51_SDK_PATH} nrf51_sdk

cd ${EHAL_PATH}/ARM/Nordic/nRF52
rm -f nrf52_sdk
ln -s ${NRF52_SDK_PATH} nrf52_sdk

cd ${INSTALL_PATH}/sdk

rm -f ehal_latest
ln -s ${EHAL_PATH} ehal_latest

rm -f nrf51_sdk_latest
ln -s ${NRF51_SDK_PATH} nrf51_sdk_latest

rm -f nrf52_sdk_latest
ln -s ${NRF52_SDK_PATH} nrf52_sdk_latest
