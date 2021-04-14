library ieee;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
entity zjw_ALU is
port(sel:in std_logic_vector(3 downto 0);
     a,b:in std_logic_vector(7 downto 0);
     c:out std_logic_vector(7 downto 0));
end zjw_ALU;
architecture rtl of zjw_ALU is
begin
c<=(others=>'0')               when sel=x"0" else
   a and b                     when sel=x"1" else
   a or b                      when sel=x"2" else
   not a                       when sel=x"3" else
   a xnor b                    when sel=x"4" else
   a xor b                     when sel=x"5" else
   a+b                         when sel=x"6" else
   a-b                         when sel=x"7" else
   a+b-((not a) and b)+b       when sel=x"8" else
   not((a xnor b)-(a xor b))+1 when sel=x"9" else
   (others=>'Z');
end rtl;
   