The codec gets reused by just keeping the original content of the V3.1.1 firmware. The files fw_311_0x00004400_0x00007fff.dat and fw_311_0x00050000_0x0007afff.dat get placed within the flash by the linker and must contain original decrypted areas of the FW V3.1.1 firmware.

To avoid copyright issues the files are not included in this repository. Before biulding the correct data has to get manually created by running "codec_dat_files_creator.exe". It downloads a FW V3.1.1 file and creates the necessary *.dat files.

- 0x00000400 to 0x00003fff from a decrypted FW V3.1.1 file => fw_311_0x00004400_0x00007fff.dat.
- 0x0004c000 to 0x00076fff from a decrypted FW V3.1.1 file => fw_311_0x00050000_0x0007afff.dat.
