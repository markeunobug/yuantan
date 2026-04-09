module AD_ROM
(
	input in_rom_clk,	//1.2M鏃堕挓杈撳叆锛屽搴?k鐮佽〃锛岄渶姹傝姹?k鐮佽〃锛岃繖閲岄渶瑕?00k鏃堕挓锛屾晠鍏堣浆鎹?
	input rst_n,
	input sync_reset,
	output [15:0] sinrom_1k,
	output [15:0] sinrom_4k,
	output [15:0] sinrom_20k,
	output [15:0] sinrom_50k,

	output [15:0] cosrom_1k,
	output [15:0] cosrom_4k,
	output [15:0] cosrom_20k,
	output [15:0] cosrom_50k
);
wire rom_clk;

assign rom_clk=in_rom_clk;

reg [10:0] Rom_addr_1k;
reg [10:0] Rom_addr_4k;
reg [10:0] Rom_addr_20k;
reg [10:0] Rom_addr_50k;

wire [10:0] Rom_addr_sin_1k;
wire [10:0] Rom_addr_sin_4k;
wire [10:0] Rom_addr_sin_20k;
wire [10:0] Rom_addr_sin_50k;

wire [10:0] Rom_addr_cos_1k;
wire [10:0] Rom_addr_cos_4k;
wire [10:0] Rom_addr_cos_20k;
wire [10:0] Rom_addr_cos_50k;

assign Rom_addr_cos_1k   = Rom_addr_1k;
assign Rom_addr_cos_4k   = Rom_addr_4k;
assign Rom_addr_cos_20k  = Rom_addr_20k;
assign Rom_addr_cos_50k  = Rom_addr_50k;

assign Rom_addr_sin_1k   = Rom_addr_1k;
assign Rom_addr_sin_4k   = Rom_addr_4k;
assign Rom_addr_sin_20k  = Rom_addr_20k;
assign Rom_addr_sin_50k  = Rom_addr_50k;

always @(posedge rom_clk or negedge rst_n)
begin
	if(!rst_n) begin
		Rom_addr_1k 	<= 11'd0;
		Rom_addr_4k 	<= 11'd0;
		Rom_addr_20k 	<= 11'd0;
		Rom_addr_50k 	<= 11'd0;
	end
	else if(sync_reset) begin
		Rom_addr_1k 	<= 11'd0;
		Rom_addr_4k 	<= 11'd0;
		Rom_addr_20k 	<= 11'd0;
		Rom_addr_50k 	<= 11'd0;
	end
	else begin
		//鏃犳硶鏁撮櫎锛屽悗缁В鍐?
		Rom_addr_1k   <= (Rom_addr_1k   + 11'd1 ) % 11'd1200;
		Rom_addr_4k   <= (Rom_addr_4k   + 11'd4 ) % 11'd1200;
		Rom_addr_20k  <= (Rom_addr_20k  + 11'd20) % 11'd1200;
		Rom_addr_50k  <= (Rom_addr_50k  + 11'd50) % 11'd1200;
	end
end

AD_ROM_SIN_IP	sin_rom_1k (
	.clock		( rom_clk		),
	.address	( Rom_addr_sin_1k	),
	.q			( sinrom_1k	)
);
AD_ROM_COS_IP	cos_rom_1k (
	.clock		( rom_clk		),
	.address	( Rom_addr_cos_1k	),
	.q			( cosrom_1k	)
);
AD_ROM_SIN_IP	sin_rom_4k (
	.clock		( rom_clk		),
	.address	( Rom_addr_sin_4k	),
	.q			( sinrom_4k	)
);
AD_ROM_COS_IP	cos_rom_4k (
	.clock		( rom_clk		),
	.address	( Rom_addr_cos_4k	),
	.q			( cosrom_4k	)
);
AD_ROM_SIN_IP	sin_rom_20k (
	.clock		( rom_clk		),
	.address	( Rom_addr_sin_20k	),
	.q			( sinrom_20k	)
);
AD_ROM_COS_IP	cos_rom_20k (
	.clock		( rom_clk		),
	.address	( Rom_addr_cos_20k	),
	.q			( cosrom_20k	)
);
AD_ROM_SIN_IP	sin_rom_50k (
	.clock		( rom_clk		),
	.address	( Rom_addr_sin_50k	),
	.q			( sinrom_50k	)
);
AD_ROM_COS_IP	cos_rom_50k (
	.clock		( rom_clk		),
	.address	( Rom_addr_cos_50k	),
	.q			( cosrom_50k	)
);

endmodule
