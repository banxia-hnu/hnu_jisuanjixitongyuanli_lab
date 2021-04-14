library IEEE;
use IEEE.std_logic_1164.all;
entity decoder16 is
port (cin:in STD_LOGIC_VECTOR(3 downto 0);
      a,b,c,d,e,f,g:out STD_LOGIC);
end decoder16;
architecture rtl of decoder16 is
begin
 process(cin)
 begin
 if (cin="0000") then
 a <='1';b <='1';c <='1';d <='1';e <='1';f <='1';g <='0';
 elsif (cin="0001") then
 a <='0';b <='1';c <='1';d <='0';e <='0';f <='0';g <='0';
 elsif (cin="0010") then
 a <='1';b <='1';c <='0';d <='1';e <='1';f <='0';g <='1';
 elsif (cin="0011") then
 a <='1';b <='1';c <='1';d <='1';e <='0';f <='0';g <='1';
 elsif (cin="0100") then
 a <='0';b <='1';c <='1';d <='0';e <='0';f <='1';g <='1';
 elsif (cin="0101") then
 a <='1';b <='0';c <='1';d <='1';e <='0';f <='1';g <='1';
 elsif (cin="0110") then
 a <='1';b <='0';c <='1';d <='1';e <='1';f <='1';g <='1';
 elsif (cin="0111") then
 a <='1';b <='1';c <='1';d <='0';e <='0';f <='0';g <='0';
 elsif (cin="1000") then
 a <='1';b <='1';c <='1';d <='1';e <='1';f <='1';g <='1';
 elsif (cin="1001") then
 a <='1';b <='1';c <='1';d <='1';e <='0';f <='1';g <='1';
 elsif (cin="1010") then
 a <='1';b <='1';c <='1';d <='1';e <='1';f <='0';g <='1';
 elsif (cin="1011") then
 a <='0';b <='0';c <='1';d <='1';e <='1';f <='1';g <='1';
 elsif (cin="1100") then
 a <='1';b <='0';c <='0';d <='1';e <='1';f <='1';g <='0';
 elsif (cin="1101") then
 a <='0';b <='1';c <='1';d <='1';e <='1';f <='0';g <='1';
 elsif (cin="1110") then
 a <='1';b <='0';c <='0';d <='1';e <='1';f <='1';g <='1';
 else
 a <='1';b <='0';c <='0';d <='0';e <='1';f <='1';g <='1';
 end if;
 end process;
end rtl;
        
        
