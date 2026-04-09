module AD_ROM
(
    input        clk_10m,       // 输入10MHz时钟
    input        clk_1m,        // 输入1MHz时钟
    input        rst_n,         // 复位信号，低电平有效
    
    // 低频信号输出（1K、4K、20K、80K）
    output [15:0] sinrom_1k,
    output [15:0] cosrom_1k,
    output [15:0] sinrom_4k,
    output [15:0] cosrom_4k,
    output [15:0] sinrom_20k,
    output [15:0] cosrom_20k,
    output [15:0] sinrom_50k,
    output [15:0] cosrom_50k,
    
    // 高频信号输出（400K、2M）
    output [15:0] sinrom_400k,
    output [15:0] cosrom_400k,
    output [15:0] sinrom_2M,
    output [15:0] cosrom_2M
);



// -------------------------- 2. 地址计数器定义（10位，对应1000点ROM） --------------------------
// 低频地址（1K、4K、16K、64K）：由内部生成的1MHz时钟驱动
reg [9:0] Rom_addr_1k;
reg [9:0] Rom_addr_4k;
reg [9:0] Rom_addr_20k;
reg [9:0] Rom_addr_50k;

// 高频地址（400K、2M）：由10MHz时钟驱动
reg [9:0] Rom_addr_400k;
reg [9:0] Rom_addr_2M;

// -------------------------- 3. 地址 --------------------------
// 正弦地址直接使用计数器值
wire [9:0] Rom_addr_sin_1k    = Rom_addr_1k;
wire [9:0] Rom_addr_sin_4k    = Rom_addr_4k;
wire [9:0] Rom_addr_sin_20k   = Rom_addr_20k;
wire [9:0] Rom_addr_sin_50k   = Rom_addr_50k;
wire [9:0] Rom_addr_sin_400k  = Rom_addr_400k;
wire [9:0] Rom_addr_sin_2M    = Rom_addr_2M;

wire [9:0] Rom_addr_cos_1k    = Rom_addr_1k;
wire [9:0] Rom_addr_cos_4k    = Rom_addr_4k;
wire [9:0] Rom_addr_cos_20k   = Rom_addr_20k;
wire [9:0] Rom_addr_cos_50k   = Rom_addr_50k;
wire [9:0] Rom_addr_cos_400k  = Rom_addr_400k;
wire [9:0] Rom_addr_cos_2M    = Rom_addr_2M;

// -------------------------- 4. 地址计数器逻辑（步长计算基于1000点ROM） --------------------------
// 低频地址更新（内部1MHz时钟驱动）
always @(posedge clk_1m or negedge rst_n) begin
    if (!rst_n) begin
        Rom_addr_1k   <= 10'd0;
        Rom_addr_4k   <= 10'd0;
        Rom_addr_20k  <= 10'd0;
        Rom_addr_50k  <= 10'd0;
    end else begin
        Rom_addr_1k   <= (Rom_addr_1k + 10'd1)   % 10'd1000;  // 1K：步长1 → 1MHz×1/1000=1KHz
        Rom_addr_4k   <= (Rom_addr_4k + 10'd4)   % 10'd1000;  // 4K：步长4 → 1MHz×4/1000=4KHz
        Rom_addr_20k  <= (Rom_addr_20k + 10'd20) % 10'd1000;  // 20K：步长20 → 1MHz×20/1000=20KHz
        Rom_addr_50k  <= (Rom_addr_50k + 10'd50) % 10'd1000;  // 50K：步长50 → 1MHz×50/1000=50KHz
    end
end

// 高频地址更新（10MHz时钟驱动）
always @(posedge clk_10m or negedge rst_n) begin
    if (!rst_n) begin
        Rom_addr_400k <= 10'd0;
        Rom_addr_2M   <= 10'd0;
    end else begin
        Rom_addr_400k <= (Rom_addr_400k + 10'd40)  % 10'd1000;  // 400K：步长40 → 10MHz×40/1000=400KHz
        Rom_addr_2M   <= (Rom_addr_2M + 10'd200)   % 10'd1000;  // 2M：步长200 → 10MHz×200/1000=2MHz
    end
end

// -------------------------- 5. ROM IP实例化（时钟与地址计数器驱动时钟一致） --------------------------
// 1KHz
AD_ROM_SIN_IP sin_rom_1k (
    .clock  (clk_1m),
    .address(Rom_addr_sin_1k),
    .q      (sinrom_1k)
);
AD_ROM_COS_IP cos_rom_1k (
    .clock  (clk_1m),
    .address(Rom_addr_cos_1k),
    .q      (cosrom_1k)
);

// 4KHz
AD_ROM_SIN_IP sin_rom_4k (
    .clock  (clk_1m),
    .address(Rom_addr_sin_4k),
    .q      (sinrom_4k)
);
AD_ROM_COS_IP cos_rom_4k (
    .clock  (clk_1m),
    .address(Rom_addr_cos_4k),
    .q      (cosrom_4k)
);

// 20KHz
AD_ROM_SIN_IP sin_rom_20k (
    .clock  (clk_1m),
    .address(Rom_addr_sin_20k),
    .q      (sinrom_20k)
);
AD_ROM_COS_IP cos_rom_20k (
    .clock  (clk_1m),
    .address(Rom_addr_cos_20k),
    .q      (cosrom_20k)
);

// 50KHz
AD_ROM_SIN_IP sin_rom_50k (
    .clock  (clk_1m),
    .address(Rom_addr_sin_50k),
    .q      (sinrom_50k)
);
AD_ROM_COS_IP cos_rom_50k (
    .clock  (clk_1m),
    .address(Rom_addr_cos_50k),
    .q      (cosrom_50k)
);

// 400KHz
AD_ROM_SIN_IP sin_rom_400k (
    .clock  (clk_10m),
    .address(Rom_addr_sin_400k),
    .q      (sinrom_400k)
);
AD_ROM_COS_IP cos_rom_400k (
    .clock  (clk_10m),
    .address(Rom_addr_cos_400k),
    .q      (cosrom_400k)
);

// 2MHz
AD_ROM_SIN_IP sin_rom_2M (
    .clock  (clk_10m),
    .address(Rom_addr_sin_2M),
    .q      (sinrom_2M)
);
AD_ROM_COS_IP cos_rom_2M (
    .clock  (clk_10m),
    .address(Rom_addr_cos_2M),
    .q      (cosrom_2M)
);

endmodule




// module AD_ROM
// (
// 	input rom_clk,	
// 	input rst_n,
// 	output [15:0] sinrom_4k,
// 	output [15:0] sinrom_10k,
// 	output [15:0] sinrom_40k,
// 	output [15:0] sinrom_100k,
// 	output [15:0] sinrom_400k,
// 	output [15:0] cosrom_4k,
// 	output [15:0] cosrom_10k,
// 	output [15:0] cosrom_40k,
// 	output [15:0] cosrom_100k,
// 	output [15:0] cosrom_400k
// );

// reg [9:0] Rom_addr_4k;
// reg [9:0] Rom_addr_10k;
// reg [9:0] Rom_addr_40k;
// reg [9:0] Rom_addr_100k;
// reg [9:0] Rom_addr_400k;

// wire [9:0] Rom_addr_sin_4k;
// wire [9:0] Rom_addr_sin_10k;
// wire [9:0] Rom_addr_sin_40k;
// wire [9:0] Rom_addr_sin_100k;
// wire [9:0] Rom_addr_sin_400k;

// wire [9:0] Rom_addr_cos_4k;
// wire [9:0] Rom_addr_cos_10k;
// wire [9:0] Rom_addr_cos_40k;
// wire [9:0] Rom_addr_cos_100k;
// wire [9:0] Rom_addr_cos_400k;

// assign Rom_addr_cos_4k   = Rom_addr_4k;
// assign Rom_addr_cos_10k  = Rom_addr_10k;
// assign Rom_addr_cos_40k  = Rom_addr_40k;
// assign Rom_addr_cos_100k = Rom_addr_100k;
// assign Rom_addr_cos_400k = Rom_addr_400k;

// assign Rom_addr_sin_4k   = Rom_addr_4k;
// assign Rom_addr_sin_10k  = Rom_addr_10k;
// assign Rom_addr_sin_40k  = Rom_addr_40k;
// assign Rom_addr_sin_100k = Rom_addr_100k;
// assign Rom_addr_sin_400k = Rom_addr_400k;


// always @(posedge rom_clk or negedge rst_n)
// begin
// 	if(!rst_n) begin
// 		Rom_addr_4k 	<= 10'd0;
// 		Rom_addr_10k 	<= 10'd0;
// 		Rom_addr_40k 	<= 10'd0;
// 		Rom_addr_100k  <= 10'd0;
// 		Rom_addr_400k  <= 10'd0;
// 	end
// 	else begin
// 		Rom_addr_4k   <= (Rom_addr_4k   + 10'd2  ) % 10'd600;
// 		Rom_addr_10k  <= (Rom_addr_10k  + 10'd5  ) % 10'd600;
// 		Rom_addr_40k  <= (Rom_addr_40k  + 10'd20 ) % 10'd600;
// 		Rom_addr_100k <= (Rom_addr_100k + 10'd50 ) % 10'd600;
// 		Rom_addr_400k <= (Rom_addr_400k + 10'd200)  % 10'd600;
// 	end
// end

// AD_ROM_SIN_IP	sin_rom_4k (
// 	.clock		( rom_clk		),
// 	.address	( Rom_addr_sin_4k	),
// 	.q			( sinrom_4k	)
// );
// AD_ROM_COS_IP	cos_rom_4k (
// 	.clock		( rom_clk		),
// 	.address	( Rom_addr_cos_4k	),
// 	.q			( cosrom_4k	)
// );
// AD_ROM_SIN_IP	sin_rom_10k (
// 	.clock		( rom_clk		),
// 	.address	( Rom_addr_sin_10k	),
// 	.q			( sinrom_10k	)
// );
// AD_ROM_COS_IP	cos_rom_10k (
// 	.clock		( rom_clk		),
// 	.address	( Rom_addr_cos_10k	),
// 	.q			( cosrom_10k	)
// );
// AD_ROM_SIN_IP	sin_rom_40k (
// 	.clock		( rom_clk		),
// 	.address	( Rom_addr_sin_40k	),
// 	.q			( sinrom_40k	)
// );
// AD_ROM_COS_IP	cos_rom_40k (
// 	.clock		( rom_clk		),
// 	.address	( Rom_addr_cos_40k	),
// 	.q			( cosrom_40k	)
// );
// AD_ROM_SIN_IP	sin_rom_100k (
// 	.clock		( rom_clk		),
// 	.address	( Rom_addr_sin_100k	),
// 	.q			( sinrom_100k	)
// );
// AD_ROM_COS_IP	cos_rom_100k (
// 	.clock		( rom_clk		),
// 	.address	( Rom_addr_cos_100k	),
// 	.q			( cosrom_100k	)
// );
// AD_ROM_SIN_IP	sin_rom_400k (
// 	.clock		( rom_clk		),
// 	.address	( Rom_addr_sin_400k	),
// 	.q			( sinrom_400k	)
// );
// AD_ROM_COS_IP	cos_rom_400k (
// 	.clock		( rom_clk		),
// 	.address	( Rom_addr_cos_400k	),
// 	.q			( cosrom_400k	)
// );

// endmodule
