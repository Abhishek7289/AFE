--Copyright 1986-2017 Xilinx, Inc. All Rights Reserved.
----------------------------------------------------------------------------------
--Tool Version: Vivado v.2017.2.1 (win64) Build 1957588 Wed Aug  9 16:32:24 MDT 2017
--Date        : Wed Mar 26 13:54:36 2025
--Host        : DESKTOP-CKNETAD running 64-bit major release  (build 9200)
--Command     : generate_target design_1_wrapper.bd
--Design      : design_1_wrapper
--Purpose     : IP block netlist
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity design_1_wrapper is
  port (
    CLR : out STD_LOGIC;
    CS : out STD_LOGIC;
    DIN : out STD_LOGIC;
    SCLK : out STD_LOGIC;
    from_chip_tri_i : in STD_LOGIC_VECTOR ( 19 downto 0 );
    iic_fmc_scl_io : inout STD_LOGIC;
    iic_fmc_sda_io : inout STD_LOGIC;
    led_8bits_tri_o : out STD_LOGIC_VECTOR ( 7 downto 0 );
    pcb_ctrl_tri_o : out STD_LOGIC_VECTOR ( 7 downto 0 );
    push_buttons_5bits_tri_i : in STD_LOGIC_VECTOR ( 4 downto 0 );
    reset : in STD_LOGIC;
    sys_diff_clock_clk_n : in STD_LOGIC;
    sys_diff_clock_clk_p : in STD_LOGIC;
    to_chip_tri_o : out STD_LOGIC_VECTOR ( 4 downto 0 );
    usb_uart_rxd : in STD_LOGIC;
    usb_uart_txd : out STD_LOGIC
  );
end design_1_wrapper;

architecture STRUCTURE of design_1_wrapper is
  component design_1 is
  port (
    usb_uart_rxd : in STD_LOGIC;
    usb_uart_txd : out STD_LOGIC;
    IIC_FMC_scl_i : in STD_LOGIC;
    IIC_FMC_scl_o : out STD_LOGIC;
    IIC_FMC_scl_t : out STD_LOGIC;
    IIC_FMC_sda_i : in STD_LOGIC;
    IIC_FMC_sda_o : out STD_LOGIC;
    IIC_FMC_sda_t : out STD_LOGIC;
    push_buttons_5bits_tri_i : in STD_LOGIC_VECTOR ( 4 downto 0 );
    led_8bits_tri_o : out STD_LOGIC_VECTOR ( 7 downto 0 );
    TO_CHIP_tri_o : out STD_LOGIC_VECTOR ( 4 downto 0 );
    FROM_CHIP_tri_i : in STD_LOGIC_VECTOR ( 19 downto 0 );
    PCB_CTRL_tri_o : out STD_LOGIC_VECTOR ( 7 downto 0 );
    CS : out STD_LOGIC;
    SCLK : out STD_LOGIC;
    DIN : out STD_LOGIC;
    CLR : out STD_LOGIC;
    sys_diff_clock_clk_n : in STD_LOGIC;
    sys_diff_clock_clk_p : in STD_LOGIC;
    reset : in STD_LOGIC
  );
  end component design_1;
  component IOBUF is
  port (
    I : in STD_LOGIC;
    O : out STD_LOGIC;
    T : in STD_LOGIC;
    IO : inout STD_LOGIC
  );
  end component IOBUF;
  signal iic_fmc_scl_i : STD_LOGIC;
  signal iic_fmc_scl_o : STD_LOGIC;
  signal iic_fmc_scl_t : STD_LOGIC;
  signal iic_fmc_sda_i : STD_LOGIC;
  signal iic_fmc_sda_o : STD_LOGIC;
  signal iic_fmc_sda_t : STD_LOGIC;
begin
design_1_i: component design_1
     port map (
      CLR => CLR,
      CS => CS,
      DIN => DIN,
      FROM_CHIP_tri_i(19 downto 0) => from_chip_tri_i(19 downto 0),
      IIC_FMC_scl_i => iic_fmc_scl_i,
      IIC_FMC_scl_o => iic_fmc_scl_o,
      IIC_FMC_scl_t => iic_fmc_scl_t,
      IIC_FMC_sda_i => iic_fmc_sda_i,
      IIC_FMC_sda_o => iic_fmc_sda_o,
      IIC_FMC_sda_t => iic_fmc_sda_t,
      PCB_CTRL_tri_o(7 downto 0) => pcb_ctrl_tri_o(7 downto 0),
      SCLK => SCLK,
      TO_CHIP_tri_o(4 downto 0) => to_chip_tri_o(4 downto 0),
      led_8bits_tri_o(7 downto 0) => led_8bits_tri_o(7 downto 0),
      push_buttons_5bits_tri_i(4 downto 0) => push_buttons_5bits_tri_i(4 downto 0),
      reset => reset,
      sys_diff_clock_clk_n => sys_diff_clock_clk_n,
      sys_diff_clock_clk_p => sys_diff_clock_clk_p,
      usb_uart_rxd => usb_uart_rxd,
      usb_uart_txd => usb_uart_txd
    );
iic_fmc_scl_iobuf: component IOBUF
     port map (
      I => iic_fmc_scl_o,
      IO => iic_fmc_scl_io,
      O => iic_fmc_scl_i,
      T => iic_fmc_scl_t
    );
iic_fmc_sda_iobuf: component IOBUF
     port map (
      I => iic_fmc_sda_o,
      IO => iic_fmc_sda_io,
      O => iic_fmc_sda_i,
      T => iic_fmc_sda_t
    );
end STRUCTURE;
