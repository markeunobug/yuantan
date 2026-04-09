module SIN_ROM_DA
(
	input          rom_clk,		        			//ROM时钟，与DA的更新速率同步			
	input          sys_rst,                		//复位信号
	output [7:0]   sinrom_4k,               	//输出4k码表
	output [7:0]   sinrom_10k,              	//输出10k码表
	output [7:0]   sinrom_40k,              	//输出40k码表
	output [7:0]   sinrom_100k,             	//输出100k码表
	output [7:0]   sinrom_400k               	//输出400k码表
);

reg [10:0] Rom_addr_4k;
reg [10:0] Rom_addr_10k;
reg [10:0] Rom_addr_40k;
reg [10:0] Rom_addr_100k;
reg [10:0] Rom_addr_400k;

always @(posedge rom_clk or negedge sys_rst)
begin
	if(!sys_rst) begin
		Rom_addr_4k   <= 11'd0;
		Rom_addr_10k  <= 11'd0;
		Rom_addr_40k  <= 11'd0;
		Rom_addr_100k <= 11'd0;
		Rom_addr_400k <= 11'd0;
	end
	else begin
		Rom_addr_4k   <= (Rom_addr_4k   + 11'd2  )  % 12'd2000;
		Rom_addr_10k  <= (Rom_addr_10k  + 11'd5  )  % 12'd2000;
		Rom_addr_40k  <= (Rom_addr_40k  + 11'd20 )  % 12'd2000;
		Rom_addr_100k <= (Rom_addr_100k + 11'd50 )  % 12'd2000;
		Rom_addr_400k <= (Rom_addr_400k + 11'd200)  % 12'd2000;
	end
end

DA_ROM_SIN_IP	sin_rom_4k (
	.clock		( rom_clk		),
	.address	( Rom_addr_4k	),
	.q			( sinrom_4k		)
);
DA_ROM_SIN_IP	sin_rom_10k (
	.clock		( rom_clk		),
	.address	( Rom_addr_10k	),
	.q			( sinrom_10k	)
);
DA_ROM_SIN_IP	sin_rom_40k (
	.clock		( rom_clk		),
	.address	( Rom_addr_40k	),
	.q			( sinrom_40k	)
);
DA_ROM_SIN_IP	sin_rom_100k (
	.clock		( rom_clk		),
	.address	( Rom_addr_100k),
	.q			( sinrom_100k	)
);
DA_ROM_SIN_IP	sin_rom_400k (
	.clock		( rom_clk		),
	.address	( Rom_addr_400k),
	.q			( sinrom_400k	)
);
//DA_ROM_SIN_IP	sin_rom_2M (
//	.clock		( rom_clk		),
//	.address	( Rom_addr_2M),
//	.q			( sinrom_2M	)
//);

endmodule
