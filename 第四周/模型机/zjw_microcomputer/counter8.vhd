library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.STD_LOGIC_ARITH.ALL;
entity counter8 is
port (	clk:in STD_LOGIC;
        sel:out STD_LOGIC_VECTOR(2 DOWNTO 0) );
        end counter8;
architecture Behavioral of counter8 is
signal iq: integer ;
begin
process (clk ,iq)
begin
if clk'event and clk='1' then
if iq=7 then iq<=0;
else iq<=iq+1;
end if;
end if;
sel<=conv_std_logic_vector (iq,3);
end process;
end Behavioral;