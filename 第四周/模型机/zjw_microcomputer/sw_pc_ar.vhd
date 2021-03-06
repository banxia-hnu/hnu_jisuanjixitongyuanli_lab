library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
entity sw_pc_ar is
port(Clk_cdu,pcclr,pcld,pcen	:in std_logic;
	 sw_bus,pc_bus,ldar			:in std_logic;
	 inputd						:in std_logic_vector(7 downto 0);
	 arout,pcout				:out std_logic_vector(7 downto 0);
	 d							:inout std_logic_vector(7 downto 0));
end sw_pc_ar;
architecture rtl of sw_pc_ar is
signal pc,ar,bus_Reg:std_logic_vector(7 downto 0);
begin
seq1:process(Clk_cdu,ldar,bus_Reg)
	begin
		if Clk_cdu'event and Clk_cdu='0'then
			if ldar='1'then
				ar<=bus_Reg;--ar<=pc
			end if;
		end if;
	end process;
seg2:process(Clk_cdu,pcclr,pcld,pcen,bus_Reg)
	begin
		if pcclr='0' then
			pc<=(others=>'0');
		elsif Clk_cdu'event and Clk_cdu='1'then
			if (pcld='0' and pcen='1')then
				pc<=bus_Reg;
			elsif (pcld='1' and pcen='1') then
				pc<=pc+1;
			end if;
		end if;
	end process;
bus_Reg<=inputd when(sw_bus='0' and pc_bus='1') else
	     pc  when(sw_bus='1' and pc_bus='0') else
	     d;
d<=bus_Reg when(sw_bus='0' or pc_bus='0') else
		(others=>'Z');
arout<=ar;
pcout<=pc;
end rtl;
