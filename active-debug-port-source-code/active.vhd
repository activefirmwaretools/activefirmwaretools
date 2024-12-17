library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity Active_Module is
    Port (
        SYS_CLOCK      : in  std_logic;                      -- system clock
        ACTIVE_MESSAGE : in  std_logic_vector(511 downto 0); -- 64 bytes array (64 * 8 = 512 bits)
        ACTIVE_CHANNEL : in  std_logic_vector(5 downto 0);   -- 6-bit variable
        ACTIVE_WR      : in  std_logic;                      -- single bit variable
        ACTIVE_CLOCK   : out std_logic;                      -- single bit output
        ACTIVE_DATA    : out std_logic                       -- single bit output
    );
end Active_Module;

architecture Behavioral of Active_Module is
    signal total_message_length        : integer := 0;
    signal bit_counter                 : integer := 0;
    signal byte_counter                : integer := 0;
    signal state                       : integer := 0;
    signal active_message_byte_index   : integer := 0;
    signal latched_message             : std_logic_vector(511 downto 0);
    signal first_byte                  : std_logic_vector(7 downto 0);
    signal second_byte                 : std_logic_vector(7 downto 0);
    signal next_byte                   : std_logic_vector(7 downto 0);
begin

    -- Data sending process
    process(SYS_CLOCK)
    begin
        if falling_edge(SYS_CLOCK) then
            if ACTIVE_WR = '1' then
                -- Reset counters and prepare to send data
                byte_counter <= 0;
                bit_counter <= 0;       
                total_message_length <= 0;

                first_byte <= "01111111";
                second_byte <= "01" & ACTIVE_CHANNEL;
                
                -- Calculate total_message_length
                for i in 1 to 63 loop
                    if ACTIVE_MESSAGE((i*8) + 7 downto (i*8)) = "00000000" then
                        total_message_length <= i + 2;     -- Add the header bytes
                        exit;
                    end if;
                end loop;

                -- Store the message to be sent
                latched_message <= ACTIVE_MESSAGE;
                
                state <= 0;

            elsif byte_counter < total_message_length then
            
                if state = 0 then
                    state <= 1;
                    -- Output the next bit
                    ACTIVE_CLOCK <= '0';
                    if byte_counter = 0 then
                        ACTIVE_DATA <= first_byte(7 - bit_counter);
                    elsif byte_counter = 1 then
                        ACTIVE_DATA <= second_byte(7 - bit_counter);
                        active_message_byte_index <= total_message_length - 3;  -- start past the beginning 0
                    else
                        ACTIVE_DATA <= next_byte(7 - bit_counter);
                    end if;


                else
                    ACTIVE_CLOCK <= '1';
                    state <= 0;
                    -- Update the bit and byte counters
                    if bit_counter = 7 then
                        bit_counter <= 0;
                        byte_counter <= byte_counter + 1;
                        active_message_byte_index <= active_message_byte_index - 1;
                        next_byte <= latched_message((active_message_byte_index * 8) + 7 downto (active_message_byte_index * 8));
                    else
                        bit_counter <= bit_counter + 1;
                    end if;                
                end if;
            else
                ACTIVE_CLOCK <= '1';
            end if;
        end if;
    end process;

end Behavioral;
