module Phase_Sentive_Detection(
	//系统时钟
	input 		 in_clk,
	
	//同步信号，本高频板子输出给从低频板
	output		 sync_out,

	//ADS9218 SPI配置
	// output 		SPI1_EN,
	// output		SPI1_CS,
	// output		SPI2_CS,
	//output		SPI1_SCLK,
	//output		SPI1_MOSI,	//菊花链
	//input		SPI2_MISO,
	
	//ADS9218 采样触发时钟
	output 		ADS9218_SMPCLK1,
	output 		ADS9218_SMPCLK2,

	//ADS9218 LVDS数据接口
	input			FCLK1,
	input			DCLK1,
	input			DOUTA1,
	input			DOUTB1,

	input			FCLK2,
	input			DCLK2,
	input			DOUTA2,
	input			DOUTB2,

	//DAC7821接口
	output [11:0]DAC7821_PIN,
	output		 DAC7821_CS,

	//AD5543接口
	output 		AD5543_CLK,
	output 		AD5543_SDI,
	output 		AD5543_CS,

	//双口RAM与DSP数据交互
	input 		WE,							//写信号
	input		RE,							//读信号
	input		bus_clk,					//时钟线
	inout [15:0]data_pin,					//数据线
	input [6:0] addr_pin,					//地址线

	//GAMMA
	input		GAMMA_pin,	
	//相敏检波完成标志
	output 		Detection_complete_clk
);

/*****************************全局时钟与复位***************************************/
//PLL 时钟及复位信号产生
wire CLK_10MHz;
wire CLK_50MHz;
wire CLK_TEST;
wire pll_locked;
wire sys_rst_n;
PLL	PLL_inst (
	.inclk0 		( in_clk 	),
	.c0				( CLK_10MHz	),
	.c1         	( CLK_50MHz ),
	.c2		 		( CLK_TEST ),
	.locked 		( pll_locked )
	);

assign sys_rst_n = pll_locked;

wire CLK_DAC7821;									//DAC7821驱动时钟10M
wire CLK_AD5543;									//AD5543驱动时钟50M
wire PSD_ROM_CLK_10M;								//码表读取时钟10M
reg  PSD_ROM_CLK_1M;								//码表读取时钟1M

assign PSD_ROM_CLK_10M = FCLK1;//使用AD9218的FCLK作为码表读取时钟
assign CLK_DAC7821 = sys_rst_n ? CLK_10MHz : 1'b0;
assign CLK_AD5543  = sys_rst_n ? CLK_50MHz : 1'b0;

reg  [3:0] clk_div_cnt; // 分频计数器（0~4，实现10分频）
//生成1MHz时钟，用于低频码表读取
always @(posedge PSD_ROM_CLK_10M or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
        clk_div_cnt <= 4'd0;
        PSD_ROM_CLK_1M      <= 1'b0;
    end else begin
        if (clk_div_cnt == 4'd4) begin 
            clk_div_cnt <= 4'd0;
            PSD_ROM_CLK_1M      <= ~PSD_ROM_CLK_1M;
        end else begin
            clk_div_cnt <= clk_div_cnt + 1'b1;
        end
    end
end

//输出同步信号
reg [17:0] sync_div_cnt;
reg        sync_out_reg;

always @(posedge CLK_10MHz or negedge sys_rst_n) begin
    if(!sys_rst_n) begin
        sync_div_cnt <= 18'd0;
        sync_out_reg <= 1'b0;
    end
    else begin
        if(sync_div_cnt == 18'd9999) begin
            sync_div_cnt <= 18'd0;
        end
        else begin
            sync_div_cnt <= sync_div_cnt + 1'b1;
        end

        sync_out_reg <= (sync_div_cnt < 18'd1000);
    end
end

assign sync_out = sync_out_reg;

/*************************ADS9218数据读取******************************************/

//ADS9218 采样时钟分配
assign ADS9218_SMPCLK1 = sys_rst_n ? CLK_10MHz : 1'b0;	//ADS9218采样时钟1
assign ADS9218_SMPCLK2 = sys_rst_n ? CLK_10MHz : 1'b0;	//ADS9218采样时钟2

//ADS9218 SPI配置线固定
assign SPI1_SCLK = 0;
assign SPI1_MOSI = 1;

//ADS9218 读取ADC1数据
wire [17:0] data_A1/*synthesis preserve*/;
wire [17:0] data_B1/*synthesis preserve*/;
ADS9218_Data_Read ADS9218_Data_Read_u1(
	.FCLK			(FCLK1),
	.DCLK			(DCLK1),
	.DOUTA			(DOUTA1),		
	.DOUTB			(DOUTB1),
	
	.data_A			(data_A1),
	.data_B			(data_B1)
);

//ADS9218 读取ADC2数据
wire [17:0] data_A2/*synthesis preserve*/;
wire [17:0] data_B2/*synthesis preserve*/;
ADS9218_Data_Read ADS9218_Data_Read_u2(
	.FCLK			(FCLK2),
	.DCLK			(DCLK2),
	.DOUTA			(DOUTA2),		
	.DOUTB			(DOUTB2),
	
	.data_A			(data_A2),
	.data_B			(data_B2)
);

//ADS9218 读取数据线连接
wire [17:0] ADS9218_data1;
wire [17:0] ADS9218_data2;
wire [17:0] ADS9218_data3;
wire [17:0] ADS9218_data4;

assign ADS9218_data1 = data_A1;//400K、2M
assign ADS9218_data2 = data_B1;//400K、2M
assign ADS9218_data3 = data_A2;//400K、2M
assign ADS9218_data4 = data_B2;//1K、4K、20K、80K

/*****************************ADS9218初始化配置*************************************/

// 使用默认驱动模式，不进行初始化
// ADS9218_init #(
//     .SPI_CLK_FREQ_MHZ(4)
// )
// u_ADS9218_init(
// //系统时钟复位
// 	.clk(CLK_4MHZ),
// 	.rst_n(sys_rst_n),
	
// //ADS9218接口
// 	.SPI_EN(),
// 	.SPI_CS(),
// 	.SPI_SCLK(SPI1_SCLK),
// 	.SPI_MOSI(SPI1_MOSI),
// 	.SPI_MISO(SPI1_MISO),
// //状态指示	
// 	.init_done(adc_init_done)
// );

/*********************************************************************************/

/*********************************Data_GAMMA*************************************/
// /*
reg GAMMA_pin_reg1;
reg GAMMA_pin_reg2;
wire GAMMA_pin_pos;
assign GAMMA_pin_pos = (~GAMMA_pin_reg2)&GAMMA_pin_reg1;//取伽马上升沿

always @(posedge CLK_10MHz or negedge sys_rst_n) //打拍取两次
begin 
	if(!sys_rst_n)begin
		GAMMA_pin_reg1 <= 1'd0;
		GAMMA_pin_reg2 <= 1'd0;
	end
	else begin
		GAMMA_pin_reg2 <= GAMMA_pin_reg1;
		GAMMA_pin_reg1 <= GAMMA_pin;
	end
end

reg Detection_complete_reg1;
reg Detection_complete_reg2;
wire Detection_complete_pos;
assign Detection_complete_pos = (~Detection_complete_reg2)&Detection_complete_reg1;//取5ms检波完成上升沿

always @(posedge CLK_10MHz or negedge sys_rst_n) begin //打拍取两次
	if(!sys_rst_n)begin
		Detection_complete_reg1 <= 'd0;
		Detection_complete_reg2 <= 'd0;
	end
	else begin
		Detection_complete_reg2 <= Detection_complete_reg1;
		Detection_complete_reg1 <= Detection_complete_clk;
	end
end

reg [31:0] Data_GAMMA_cnt;
always @(posedge CLK_10MHz or negedge sys_rst_n) begin//伽马累计
	if(!sys_rst_n)begin
		Data_GAMMA_cnt <= 'd0;
	end
	else if(Detection_complete_pos)
		Data_GAMMA_cnt <= 'd0;
	else if(GAMMA_pin_pos)
		Data_GAMMA_cnt <= Data_GAMMA_cnt + 1;
end

always @(posedge CLK_10MHz or negedge sys_rst_n) begin//伽马寄存
	if(!sys_rst_n)
		Data_GAMMA <= 'd0;
	else if(Detection_complete_pos)
		Data_GAMMA <= Data_GAMMA_cnt;
end

/***************************双端口RAM与DSP数据交互********************************/
//从RAM读出的参数
wire [3:0] 	amplitude_low;
wire [11:0] amplitude_high;
wire [15:0] freq;

//写入RAM的参数
reg  [63:0] Data_I1;
reg  [63:0] Data_Q1;
reg  [63:0] Data_I2;
reg  [63:0] Data_Q2;	
reg  [63:0] Data_I3;
reg  [63:0] Data_Q3;
reg  [63:0] Data_I4;
reg  [63:0] Data_Q4;	
reg  [63:0] Data_I5;
reg  [63:0] Data_Q5;
reg  [63:0] Data_I6;
reg  [63:0] Data_Q6;	
reg  [63:0] Data_I7;
reg  [63:0] Data_Q7;
reg  [63:0] Data_I8;
reg  [63:0] Data_Q8;	
reg  [63:0] Data_I9;
reg  [63:0] Data_Q9;
reg  [31:0] Data_GAMMA;

Dual_RAM Dual_RAM_u(
	.rst_n 		( sys_rst_n	),
	
	.ext_clk	( in_clk	),				//B端口时钟
	.WE			( WE		),				//
	.RE			( RE		),				//低电平有效
	.bus_clk	( bus_clk	),				//bus_clk A端口时钟
	.data_pin	( data_pin	),	
	.addr_pin	( addr_pin	),				//addr_pin
		
	
	.I1			( Data_I1 	),
	.Q1			( Data_Q1 	),
	.I2			( Data_I2  	),
	.Q2			( Data_Q2  	),
	.I3			( Data_I3  	),
	.Q3			( Data_Q3  	),
	.I4			( Data_I4  	),
	.Q4			( Data_Q4  	),
	.I5			( Data_I5  	),
	.Q5			( Data_Q5  	),
	.I6			( Data_I6  	),
	.Q6			( Data_Q6  	),
	.I7			( Data_I7  	),
	.Q7			( Data_Q7  	),
	.I8			( Data_I8  	),
	.Q8			( Data_Q8  	),
	.I9			( Data_I9  	),
	.Q9			( Data_Q9  	),
	.GAMMA		( Data_GAMMA),
	
	.CMD_1		( freq 	),				//0地址控制频率		freq
	.CMD_2		( amplitude_low	),				//1地址控制幅度
	.CMD_3		( amplitude_high),
	.led		( led_RAM 	)
);

/********************************************************************************/

/**********************************DAC7821***********************************/

reg [7:0] DAC7821_ROM;
wire [3:0] DA_Amp_high;
assign DA_Amp_high = amplitude_high;

DAC7821 DAC7821_u(
    .sys_clk	(CLK_DAC7821),
    .sys_rstn	(sys_rst_n),
    //数据
    .Amp		(DA_Amp_high),                				// DSP控制的幅度字 
    .ROM_Data	(DAC7821_ROM),           			//码表
    //硬件管脚
    .DAC7821_PIN(DAC7821_PIN),        
    .DAC7821_CS	(DAC7821_CS)  
);

/**********************************AD5543***********************************/

reg [11:0] AD5543_ROM;
wire [3:0]DA_Amp_low;
assign DA_Amp_low = amplitude_low;

AD5543 AD5543_u(
	// 输入
	.sys_clk(CLK_AD5543),
	.sys_rstn(sys_rst_n),
	.start(1'd1),

	.Amp(DA_Amp_low),//幅度字
	.dac_Data(AD5543_ROM),

	// AD5543接口
	.AD5543_CLK(AD5543_CLK),
	.AD5543_SDI(AD5543_SDI),
	.AD5543_CS(AD5543_CS)
);

/********************************相敏检波码表读取******************************/
// /*
// 码表数据（新频率：1K、4K、16K、64K、400K、2M）
wire [15:0] cos_data_1k;	
wire [15:0] sin_data_1k;
wire [15:0] cos_data_4k;	
wire [15:0] sin_data_4k;
wire [15:0] cos_data_20k;	
wire [15:0] sin_data_20k;
wire [15:0] cos_data_50k;	
wire [15:0] sin_data_50k;
wire [15:0] cos_data_400k;	
wire [15:0] sin_data_400k;
wire [15:0] cos_data_2M;	
wire [15:0] sin_data_2M;

AD_ROM AD_ROM_u
(
    .clk_10m      (PSD_ROM_CLK_10M),   // 与AD_ROM模块的时钟输入端口clk_10m匹配
	.clk_1m       (PSD_ROM_CLK_1M),    // 与AD_ROM模块的时钟输入端口clk_1m匹配
    .rst_n        (sys_rst_n),
    .sinrom_1k    (sin_data_1k),
    .sinrom_4k    (sin_data_4k),
    .sinrom_20k   (sin_data_20k),
    .sinrom_50k   (sin_data_50k),
    .sinrom_400k  (sin_data_400k),
    .sinrom_2M    (sin_data_2M),
    .cosrom_1k    (cos_data_1k),
    .cosrom_4k    (cos_data_4k),
    .cosrom_20k   (cos_data_20k),
    .cosrom_50k   (cos_data_50k),
    .cosrom_400k  (cos_data_400k),
    .cosrom_2M    (cos_data_2M)
);

// 实例化码表模块
wire [15:0] DA_sin_1k;
wire [15:0] DA_sin_4k;
wire [15:0] DA_sin_20k;
wire [15:0] DA_sin_50k;
wire [15:0] DA_sin_400k;
wire [15:0] DA_sin_2M;

DA_ROM DA_ROM_u (
    .clk_10m     (PSD_ROM_CLK_10M),
    .clk_1m      (PSD_ROM_CLK_1M),
    .sys_rst     (sys_rst_n),           // 低有效复位

    .sinrom_1k   (DA_sin_1k),                 // 悬空不用的端口
    .sinrom_4k   (DA_sin_4k),
    .sinrom_20k  (DA_sin_20k),
    .sinrom_50k  (DA_sin_50k),
    .sinrom_400k (DA_sin_400k),
    .sinrom_2M   (DA_sin_2M)
);

/********************************相敏检波**************************************/
//6个PSD模块的时钟
wire PSD1_clk;
wire PSD2_clk;
wire PSD3_clk;
wire PSD4_clk;

assign PSD1_clk = FCLK1;
assign PSD2_clk = FCLK1;
assign PSD3_clk = FCLK2;
assign PSD4_clk = FCLK2;//降低采样

//PSD模块检波直接输出数据
wire [63:0] PSD_I1;
wire [63:0] PSD_Q1;
wire [63:0] PSD_I2;
wire [63:0] PSD_Q2;	
wire [63:0] PSD_I3;
wire [63:0] PSD_Q3;
wire [63:0] PSD_I4;
wire [63:0] PSD_Q4;	

//PSD模块的输入数据接口
reg  [19:0] PSD_AD1;
reg  [19:0] PSD_AD2;
reg  [19:0] PSD_AD3;
reg  [19:0] PSD_AD4;

//IQ波表
reg  [15:0] PSD_LOW_Cos_Table;//低频余弦表
reg  [15:0] PSD_LOW_Sin_Table;//低频正弦表
reg  [15:0] PSD_HIGH_Cos_Table;//高频余弦表
reg  [15:0] PSD_HIGH_Sin_Table;//高频正弦表

Phase_Sensitive Phase_Sensitive_1(
	.rst_n					( sys_rst_n	),
	.dclk					( PSD1_clk),

	.ADdata					( ADS9218_data1  ),
	.cos_rom_data			( PSD_HIGH_Cos_Table ),	
	.sin_rom_data			( PSD_HIGH_Sin_Table ),	
	.SIN_I					( PSD_I1		),			
	.COS_Q					( PSD_Q1		),			
	.Phase_Sensitive_Done	( Detection_complete_clk ) 
);
Phase_Sensitive Phase_Sensitive_2(
	.rst_n					( sys_rst_n	),
	.dclk					( PSD2_clk),

	.ADdata					( ADS9218_data2	),
	.cos_rom_data			( PSD_HIGH_Cos_Table ),	
	.sin_rom_data			( PSD_HIGH_Sin_Table ),	
	.SIN_I					( PSD_I2		),
	.COS_Q					( PSD_Q2		)	
);
Phase_Sensitive Phase_Sensitive_3(
	.rst_n					( sys_rst_n	),
	.dclk					( PSD3_clk),

	.ADdata					( ADS9218_data3	),
	.cos_rom_data			( PSD_HIGH_Cos_Table ),	
	.sin_rom_data			( PSD_HIGH_Sin_Table ),	
	.SIN_I					( PSD_I3		),
	.COS_Q					( PSD_Q3		)		
);

 Phase_Sensitive Phase_Sensitive_4(
 	.rst_n					( sys_rst_n	),
 	.dclk					( PSD4_clk),

 	.ADdata					( ADS9218_data4	),
 	.cos_rom_data			( PSD_LOW_Cos_Table ),	
 	.sin_rom_data			( PSD_LOW_Sin_Table ),	
 	.SIN_I					( PSD_I4		),
 	.COS_Q					( PSD_Q4		)	
 );

/*******************************频率切换****************************************/
parameter freq_1k		= 4'd1;
parameter freq_4k		= 4'd2;
parameter freq_20k		= 4'd4;
parameter freq_50k		= 4'd8;
parameter freq_400k		= 4'd1;
parameter freq_2M		= 4'd2;

wire [3:0] 	freq_low;
wire [1:0] 	freq_high;
assign freq_low  = freq[3:0];// 提取低四位：bit[3]到bit[0]
assign freq_high = freq[5:4];// 提取第五六位：bit[4-5]（注：位索引从0开始，第五位对应索引4）
///*
// 定义用于观测的寄存器
reg [3:0] freq_low_reg;  // 寄存freq_low的观测寄存器
reg [1:0] freq_high_reg; // 寄存freq_high的观测寄存器

// 同步时序逻辑：在10MHz时钟上升沿捕获观测值
always @(posedge CLK_10MHz or negedge sys_rst_n) begin 
    if(!sys_rst_n)begin
        // 复位时初始化寄存器为0
        freq_low_reg  <= 4'd0;
        freq_high_reg <= 2'b0;
    end
    else begin
        // 时钟上升沿捕获当前值，存入寄存器用于观测
        freq_low_reg  <= freq_low;  // 寄存低四位信号
        freq_high_reg <= freq_high; // 寄存第五位信号
    end
end
//*/

always @(posedge CLK_DAC7821 or negedge sys_rst_n) begin
	if(!sys_rst_n)begin
	  	DAC7821_ROM  <= 8'd0;			//复位时DA不输出
		AD5543_ROM  <= 12'd0;
	end
	else begin
		if(freq != 16'd0 && freq != 16'hFF)begin
		 case(freq_low_reg)
		 //case(freq_low)
			freq_1k:
					AD5543_ROM  <= DA_sin_1k[15:4];
			freq_4k:
					AD5543_ROM  <= DA_sin_4k[15:4];
			freq_20k:
					AD5543_ROM  <= DA_sin_20k[15:4];
			freq_50k:
					AD5543_ROM  <= DA_sin_50k[15:4];	
			default: 
					AD5543_ROM  <= 12'd0;
		 endcase
		 
		// case(freq_high)
		 case(freq_high_reg)
			freq_400k:
					DAC7821_ROM  <= DA_sin_400k[15:8];
			freq_2M:
					DAC7821_ROM  <= DA_sin_2M[15:8];	
			default: 
					DAC7821_ROM  <= 8'd0;
			endcase
		end
	end	
end

// /********************************码表选择****************************************/
always @(posedge FCLK1 or negedge sys_rst_n)	
begin 
    if(!sys_rst_n)
		begin
			PSD_HIGH_Cos_Table <= 16'h0;
			PSD_HIGH_Sin_Table <= 16'h0;		
			Data_I1 <= 64'd0; Data_Q1 <= 64'd0;
			Data_I2 <= 64'd0; Data_Q2 <= 64'd0;
			Data_I3 <= 64'd0; Data_Q3 <= 64'd0;
			Data_I4 <= 64'd0; Data_Q4 <= 64'd0;
			Data_I5 <= 64'd0; Data_Q5 <= 64'd0;
			Data_I6 <= 64'd0; Data_Q6 <= 64'd0;
			Data_I7 <= 64'd0; Data_Q7 <= 64'd0;
			Data_I8 <= 64'd0; Data_Q8 <= 64'd0;
			Data_I9 <= 64'd0; Data_Q9 <= 64'd0;	
		end		
	 else begin
		case(freq_low)
			freq_1k:
				begin
					PSD_LOW_Cos_Table <= cos_data_1k;
					PSD_LOW_Sin_Table <= sin_data_1k;			
				end
			freq_4k:
				begin
					PSD_LOW_Cos_Table <= cos_data_4k;
					PSD_LOW_Sin_Table <= sin_data_4k;			
				end	
			freq_20k:
				begin
					PSD_LOW_Cos_Table <= cos_data_20k;
					PSD_LOW_Sin_Table <= sin_data_20k;			
				end
			freq_50k:
				begin
					PSD_LOW_Cos_Table <= cos_data_50k;
					PSD_LOW_Sin_Table <= sin_data_50k;			
				end	
			default:
				begin
					PSD_LOW_Cos_Table <= 16'h0;
					PSD_LOW_Sin_Table <= 16'h0;			
				end
	 	endcase
		case(freq_high)
			freq_400k:
				begin
					PSD_HIGH_Cos_Table <= cos_data_400k;
					PSD_HIGH_Sin_Table <= sin_data_400k;			
				end
			freq_2M:
				begin
					PSD_HIGH_Cos_Table <= cos_data_2M;
					PSD_HIGH_Sin_Table <= sin_data_2M;			
				end	
			default:
				begin
					PSD_HIGH_Cos_Table <= 16'h0;
					PSD_HIGH_Sin_Table <= 16'h0;			
				end
	 	endcase	 
		
		Data_I1 <= PSD_I1;Data_Q1 <= PSD_Q1;
		Data_I2 <= PSD_I2;Data_Q2 <= PSD_Q2;
		Data_I3 <= PSD_I3;Data_Q3 <= PSD_Q3;
		Data_I4 <= PSD_I4;Data_Q4 <= PSD_Q4;
		Data_I5 <= 64'd0; Data_Q5 <= 64'd0;
		Data_I6 <= 64'd0; Data_Q6 <= 64'd0;
		Data_I7 <= 64'd0; Data_Q7 <= 64'd0;
		Data_I8 <= 64'd0; Data_Q8 <= 64'd0;
		Data_I9 <= 64'd0; Data_Q9 <= 64'd0;

	end

end
// */
endmodule
