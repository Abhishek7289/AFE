
## FMC
##SPI signals
set_property -dict { PACKAGE_PIN M25   IOSTANDARD LVCMOS18 } [get_ports { CS }]; #IO_L23N_T3_FWE_B_15 Sch=fmc_ha_n[04]
set_property -dict { PACKAGE_PIN J28   IOSTANDARD LVCMOS18 } [get_ports { DIN }]; #IO_L8N_T1_AD3N_15 Sch=fmc_ha_n[08]
set_property -dict { PACKAGE_PIN J27   IOSTANDARD LVCMOS18 } [get_ports { SCLK }]; #IO_L8P_T1_AD3P_15 Sch=fmc_ha_p[08]
set_property -dict { PACKAGE_PIN L26   IOSTANDARD LVCMOS18 } [get_ports { CLR }]; #IO_L11P_T1_SRCC_AD12P_15 Sch=fmc_ha_p[12]
##PCB control signals
set_property -dict { PACKAGE_PIN G29   IOSTANDARD LVCMOS18 } [get_ports { pcb_ctrl_tri_o[0] }]; #IO_L22P_T3_16 Sch=fmc_ha_p[19] digipot reset
set_property -dict { PACKAGE_PIN M22   IOSTANDARD LVCMOS18 } [get_ports { pcb_ctrl_tri_o[1] }]; #IO_L24P_T3_RS1_15 Sch=fmc_ha_p[16] FMC_test_led[0]
set_property -dict { PACKAGE_PIN F27   IOSTANDARD LVCMOS18 } [get_ports { pcb_ctrl_tri_o[3] }]; #IO_L21N_T3_DQS_16 Sch=fmc_ha_n[20] FMC_test_led[1]
set_property -dict { PACKAGE_PIN G27   IOSTANDARD LVCMOS18 } [get_ports { pcb_ctrl_tri_o[2] }]; #IO_L21P_T3_DQS_16 Sch=fmc_ha_p[20] FMC_test_led[2]
set_property -dict { PACKAGE_PIN D23   IOSTANDARD LVCMOS18 } [get_ports { pcb_ctrl_tri_o[4] }]; #IO_L2N_T0_16 Sch=fmc_la_n[16] DAC1_CLR_FPGA
set_property -dict { PACKAGE_PIN E23   IOSTANDARD LVCMOS18 } [get_ports { pcb_ctrl_tri_o[5] }]; #IO_L2P_T0_16 Sch=fmc_la_p[16] DAC0_CLR_FPGA
set_property -dict { PACKAGE_PIN F22   IOSTANDARD LVCMOS18 } [get_ports { pcb_ctrl_tri_o[6] }]; #IO_L9N_T1_DQS_17 Sch=fmc_la_n[20] LDAC1_FPGA
set_property -dict { PACKAGE_PIN G22   IOSTANDARD LVCMOS18 } [get_ports { pcb_ctrl_tri_o[7] }]; #IO_L9P_T1_DQS_17 Sch=fmc_la_p[20] LDAC0_FPGA

#IIC signals
set_property -dict { PACKAGE_PIN AC24  IOSTANDARD LVCMOS33 } [get_ports { iic_fmc_scl_io }]; #IO_L9P_T1_DQS_12 Sch=fmc_scl
set_property -dict { PACKAGE_PIN AD24  IOSTANDARD LVCMOS33 } [get_ports { iic_fmc_sda_io }]; #IO_L9N_T1_DQS_12 Sch=fmc_sda

#CHIP control signals INPUT to CHIP

set_property -dict { PACKAGE_PIN J14   IOSTANDARD LVCMOS18 } [get_ports { to_chip_tri_o[0] }]; #IO_L5N_T0_18 Sch=fmc_hb_n[08]  >> SEL2
set_property -dict { PACKAGE_PIN A12   IOSTANDARD LVCMOS18 } [get_ports { to_chip_tri_o[1] }]; #IO_L17N_T2_18 Sch=fmc_hb_n[12] >> SEL1
set_property -dict { PACKAGE_PIN H12   IOSTANDARD LVCMOS18 } [get_ports { to_chip_tri_o[2] }]; #IO_L10N_T1_18 Sch=fmc_hb_n[14] >> DIN[30] ACK_36
set_property -dict { PACKAGE_PIN K18   IOSTANDARD LVCMOS18 } [get_ports { to_chip_tri_o[3] }]; #IO_L1P_T0_17 Sch=fmc_la_p[32] >> DIN[31] ACK_16
set_property -dict { PACKAGE_PIN J18   IOSTANDARD LVCMOS18 } [get_ports { to_chip_tri_o[4] }]; #IO_L1N_T0_17 Sch=fmc_la_n[32] >> DIN[32] ENABLE

#CHIP  signals Output from CHIP

set_property -dict { PACKAGE_PIN E20    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[0] }]; #IO_L12N_T1_MRCC_17 Sch=fmc_clk0_m2c_n AER_16[0]
set_property -dict { PACKAGE_PIN F20    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[1] }]; #IO_L12P_T1_MRCC_17 Sch=fmc_clk0_m2c_p AER_16[1]
set_property -dict { PACKAGE_PIN N25    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[2] }]; #IO_L18P_T2_A24_15 Sch=fmc_ha_p[03] AER_16[2]
set_property -dict { PACKAGE_PIN K25    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[3]}]; #AER_16[3]
set_property -dict { PACKAGE_PIN N26    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[4]}]; #AER_36[0]
set_property -dict { PACKAGE_PIN H30    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[5]}]; #AER_36[1]
set_property -dict { PACKAGE_PIN G30    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[6]}];#AER_36[2]
set_property -dict { PACKAGE_PIN P21    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[7]}];#AER_36[3]
set_property -dict { PACKAGE_PIN M29    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[8]}];#AER_36[4]
set_property -dict { PACKAGE_PIN P22    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[9]}];#AER_36[5]
set_property -dict { PACKAGE_PIN M30    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[10]}];#AER_36[6]
set_property -dict { PACKAGE_PIN H26    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[11]}];#AER_36[7]
set_property -dict { PACKAGE_PIN H27    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[12]}]; #REQ_36
set_property -dict { PACKAGE_PIN N29    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[13]}];#REQ_16
set_property -dict { PACKAGE_PIN P23    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[14]}];
set_property -dict { PACKAGE_PIN N30    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[15]}];
set_property -dict { PACKAGE_PIN N24    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[16]}];
set_property -dict { PACKAGE_PIN F25    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[17]}];
set_property -dict { PACKAGE_PIN E25    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[18]}];
set_property -dict { PACKAGE_PIN N21    IOSTANDARD LVCMOS18} [get_ports {from_chip_tri_i[19]}];


set_property -dict { PACKAGE_PIN T28   IOSTANDARD LVCMOS33 } [get_ports { led_8bits_tri_o[0] }]; #IO_L11N_T1_SRCC_14 Sch=led[0]
set_property -dict { PACKAGE_PIN V19   IOSTANDARD LVCMOS33 } [get_ports { led_8bits_tri_o[1] }]; #IO_L19P_T3_A10_D26_14 Sch=led[1]
set_property -dict { PACKAGE_PIN U30   IOSTANDARD LVCMOS33 } [get_ports { led_8bits_tri_o[2] }]; #IO_L15N_T2_DQS_DOUT_CSO_B_14 Sch=led[2]
set_property -dict { PACKAGE_PIN U29   IOSTANDARD LVCMOS33 } [get_ports { led_8bits_tri_o[3] }]; #IO_L15P_T2_DQS_RDWR_B_14 Sch=led[3]
set_property -dict { PACKAGE_PIN V20   IOSTANDARD LVCMOS33 } [get_ports { led_8bits_tri_o[4] }]; #IO_L19N_T3_A09_D25_VREF_14 Sch=led[4]
set_property -dict { PACKAGE_PIN V26   IOSTANDARD LVCMOS33 } [get_ports { led_8bits_tri_o[5] }]; #IO_L16P_T2_CSI_B_14 Sch=led[5]
set_property -dict { PACKAGE_PIN W24   IOSTANDARD LVCMOS33 } [get_ports { led_8bits_tri_o[6] }]; #IO_L20N_T3_A07_D23_14 Sch=led[6]
set_property -dict { PACKAGE_PIN W23   IOSTANDARD LVCMOS33 } [get_ports { led_8bits_tri_o[7] }]; #IO_L20P_T3_A08_D24_14 Sch=led[7]

set_property IOSTANDARD LVCMOS18 [get_ports {push_buttons_5bits_tri_i[4]}];
set_property IOSTANDARD LVCMOS18 [get_ports {push_buttons_5bits_tri_i[3]}];
set_property IOSTANDARD LVCMOS18 [get_ports {push_buttons_5bits_tri_i[2]}];
set_property IOSTANDARD LVCMOS18 [get_ports {push_buttons_5bits_tri_i[1]}];
set_property IOSTANDARD LVCMOS18 [get_ports {push_buttons_5bits_tri_i[0]}];














