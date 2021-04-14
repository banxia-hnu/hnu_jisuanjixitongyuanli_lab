library IEEE;
use IEEE.std_logic_1164.all;
entity zjw_zongxian is
port(CLK:in std_logic;
     sw_bus,r1_bus,r2_bus,r3_bus:in std_logic;
     k:in std_logic_vector(7 downto 0);
     lddr:in std_logic_vector(2 downto 0);
     l:inout std_logic_vector(7 downto 0));
end zjw_zongxian;
architecture rtl of zjw_zongxian is
signal r1,r2,r3,bus_Reg:std_logic_vector(7 downto 0);
begin
ldreg:process(CLK,lddr,bus_Reg)
      begin
      if(CLK'event and CLK='1') then
         if lddr(0)='1' then r1<=bus_Reg;
         elsif lddr(1)='1' then r2<=bus_Reg;
         elsif lddr(2)='1' then r3<=bus_Reg;
         end if;
      end if;
      end process;
bus_Reg<=k  when(sw_bus='0' and r1_bus='1' and r2_bus='1' and r3_bus='1') else
         r1 when(sw_bus='1' and r1_bus='0' and r2_bus='1' and r3_bus='1') else
         r2 when(sw_bus='1' and r1_bus='1' and r2_bus='0' and r3_bus='1') else
         r3 when(sw_bus='1' and r1_bus='1' and r2_bus='1' and r3_bus='0') else
         (others=>'0');
l<=bus_Reg when(sw_bus='0' and r1_bus='0' and r2_bus='0' and r3_bus='0')else
   (others=>'Z');
end rtl;
