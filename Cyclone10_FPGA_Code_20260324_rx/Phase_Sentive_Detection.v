module Phase_Sentive_Detection(
	//系统时钟
	input 		 in_clk,
	
	//AD4020接口	
	output		AD4020_sck,
	output		AD4020_cnv,
		
	input 		AD4020_sdo1,
	input 		AD4020_sdo2,
	input 		AD4020_sdo3,
	input 		AD4020_sdo4,

	//双口RAM与DSP数据交互
	input 		WE,							//写信号
	input		RE,							//读信号
	input		bus_clk,					//时钟线
	inout [15:0]data_pin,					//数据线
	input [6:0] addr_pin,					//地址线
	
	//LED
	output LED,
	
	//同步信号
	input sync_in,

	//GAMMA
	input		GAMMA_pin,
	//相敏检波完成标志
	output 		Detection_complete_clk
);

/*****************************全局时钟与复位***************************************/
//PLL 时钟及复位信号产生
wire CLK_96MHZ;
wire CLK_4MHZ;
wire pll_locked;
wire sys_rst_n;
PLL	PLL_inst (
	.inclk0 		( in_clk 	),
	.c0				( CLK_96MHZ	),
	.c1         	( CLK_4MHZ ),
	.locked 		( pll_locked )
	);


wire AD4020_clk;										//AD4020驱动时钟96M
wire PSD_ROM_CLK;										//码表读取时钟1.2M

reg  sync_in_meta;
reg  sync_in_reg1;
reg  sync_in_reg2;
wire sync_in_pos;

assign sys_rst_n = pll_locked;
assign AD4020_clk = sys_rst_n ? CLK_96MHZ : 1'b0;
assign PSD_ROM_CLK = AD4020_dclk;//

always @(posedge PSD_ROM_CLK or negedge sys_rst_n) begin
	if(!sys_rst_n) begin
		sync_in_meta <= 1'b0;
		sync_in_reg1 <= 1'b0;
		sync_in_reg2 <= 1'b0;
	end
	else begin
		sync_in_meta <= sync_in;
		sync_in_reg1 <= sync_in_meta;
		sync_in_reg2 <= sync_in_reg1;
	end
end

assign sync_in_pos = sync_in_reg1 & (~sync_in_reg2);


//6个PSD模块的时钟
wire PSD1_clk;
wire PSD2_clk;
wire PSD3_clk;
wire PSD4_clk;
wire PSD5_clk;
wire PSD6_clk;
wire PSD7_clk;

assign PSD1_clk = AD4020_dclk;
assign PSD2_clk = AD4020_dclk;
assign PSD3_clk = AD4020_dclk;
assign PSD4_clk = AD4020_dclk;
assign PSD5_clk = AD4020_dclk;
assign PSD6_clk = AD4020_dclk;
assign PSD7_clk = AD4020_dclk;
/*********************************************************************************/

//LED
assign LED = 1'b0;

/*************************AD4020数据读取******************************************/
wire AD4020_dclk;										//固定为1.2MHZ,下降沿触发采集，上升沿输出的数据为1.5周期前采集

wire [19:0]AD4020_data1;
wire [19:0]AD4020_data2;
wire [19:0]AD4020_data3;
wire [19:0]AD4020_data4;


AD4020_ReadData AD4020_ReadData_9ch_u(
	.sys_clk			( AD4020_clk ),	
	.rst_n				( sys_rst_n	 ),
	//AD4020接口
	.sdo1				( AD4020_sdo1 ),
	.sdo2				( AD4020_sdo2 ),
	.sdo3				( AD4020_sdo3 ),
	.sdo4				( AD4020_sdo4 ),
	.sck				( AD4020_sck ),
	.dclk				( AD4020_dclk ),
	.cnv				( AD4020_cnv ),
	.test				(	 ),
	//数据输出接口
	.ADdata1			( AD4020_data1 ),
	.ADdata2			( AD4020_data2 ),
	.ADdata3			( AD4020_data3 ),
	.ADdata4			( AD4020_data4 )
);

/********************************************************************************/

/*********************************Data_GAMMA*************************************/

reg GAMMA_pin_reg1;
reg GAMMA_pin_reg2;
wire GAMMA_pin_pos;
assign GAMMA_pin_pos = (~GAMMA_pin_reg2)&GAMMA_pin_reg1;//取伽马上升沿

always @(posedge AD4020_clk or negedge sys_rst_n) //打拍取两次
begin 
	if(!sys_rst_n)begin
		GAMMA_pin_reg1 <= 'd0;
		GAMMA_pin_reg2 <= 'd0;
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

always @(posedge AD4020_clk) begin //打拍取两次
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
always @(posedge AD4020_clk) begin//伽马累计
	if(!sys_rst_n)begin
		Data_GAMMA_cnt <= 'd0;
	end
	else if(Detection_complete_pos)//检波完成后清零
		Data_GAMMA_cnt <= 'd0;
	else if(GAMMA_pin_pos)//伽马信号上升沿
		Data_GAMMA_cnt <= Data_GAMMA_cnt + 1;
end

always @(posedge AD4020_clk) begin//伽马寄存
	if(!sys_rst_n)
		Data_GAMMA <= 'd0;
	else if(Detection_complete_pos)//检波完成后寄存
		Data_GAMMA <= Data_GAMMA_cnt;
end

/***************************双端口RAM与DSP数据交互********************************/
//从RAM读出的参数
wire [3:0] 	amplitude;
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

wire  [63:0] test_I1;
wire  [63:0] test_Q1;

assign test_I1 = 64'h1234567890ABCDEF;	//测试数据
assign test_Q1 = 64'h5555555555555555;	//测试数据


Dual_RAM Dual_RAM_u(
	.rst_n 		( sys_rst_n	),
	
	.ext_clk	( in_clk	),				//B端口时钟
	.WE			( WE		),				//
	.RE			( RE		),				//低电平有效
	.bus_clk	( bus_clk	),				//bus_clk A端口时钟
	.data_pin	( data_pin	),	
	.addr_pin	( addr_pin	),				//addr_pin
		
	
	.I1			( Data_I1 	),//Data_I1
	.Q1			( Data_Q1 	),//Data_Q1
	.I2			( Data_I2  	),
	.Q2			( Data_Q2  	),
	.I3			( Data_I3  	),
	.Q3			( Data_Q3  	),
	.I4			( Data_I4  	),
	.Q4			( Data_Q4  	),
	.I5			( Data_I5 	),
	.Q5			( Data_Q5 	),
	.I6			( Data_I6 	),
	.Q6			( Data_Q6 	),
	.I7			( Data_I7 	),
	.Q7			( Data_Q7 	),
	.I8			( Data_I8 	),
	.Q8			( Data_Q8 	),
	.I9			( Data_I9 	),
	.Q9			( Data_Q9 	),
	.GAMMA		( Data_GAMMA),
	
	.CMD_1		( freq  	),				//0地址控制频率
	.CMD_2		( amplitude	),				//1地址控制幅度
	.CMD_3		(			),
	.led		( led_RAM 	)
);
/********************************************************************************/


/********************************相敏检波码表读取******************************/
//需求1k\4k\16k\64k
//码表数据
wire [15:0]cos_data_1k;	
wire [15:0]sin_data_1k;
wire [15:0]cos_data_4k;	
wire [15:0]sin_data_4k;
wire [15:0]cos_data_20k;	
wire [15:0]sin_data_20k;
wire [15:0]cos_data_50k;	
wire [15:0]sin_data_50k;

AD_ROM AD_ROM_u
(
	.in_rom_clk	(PSD_ROM_CLK),	
	.rst_n		(sys_rst_n),
	.sync_reset	(sync_in_pos),
	.sinrom_1k	(sin_data_1k),
	.sinrom_4k	(sin_data_4k),
	.sinrom_20k	(sin_data_20k),
	.sinrom_50k	(sin_data_50k),

	.cosrom_1k	(cos_data_1k),
	.cosrom_4k	(cos_data_4k),
	.cosrom_20k	(cos_data_20k),
	.cosrom_50k	(cos_data_50k)
);

/********************************相敏检波**************************************/
// //6个PSD模块检波直接输出数据
wire [63:0] PSD_I1;
wire [63:0] PSD_Q1;
wire [63:0] PSD_I2;
wire [63:0] PSD_Q2;	
wire [63:0] PSD_I3;
wire [63:0] PSD_Q3;
wire [63:0] PSD_I4;
wire [63:0] PSD_Q4;	
// wire [63:0] PSD_I5;
// wire [63:0] PSD_Q5;
// wire [63:0] PSD_I6;
// wire [63:0] PSD_Q6;
// wire [63:0] PSD_I7;
// wire [63:0] PSD_Q7;
// //6个PSD模块的输入数据接口
reg  [19:0] PSD_AD1;
reg  [19:0] PSD_AD2;
reg  [19:0] PSD_AD3;
reg  [19:0] PSD_AD4;
// reg  [19:0] PSD_AD5;
// reg  [19:0] PSD_AD6;
// reg  [19:0] PSD_AD7;
// //IQ波表
reg  [15:0] PSD_Cos_Table;
reg  [15:0] PSD_Sin_Table;

Phase_Sensitive Phase_Sensitive_1(
	.rst_n					( sys_rst_n	),
	.dclk					( PSD1_clk),

	.ADdata					( PSD_AD1  ),
	.cos_rom_data			( PSD_Cos_Table ),	
	.sin_rom_data			( PSD_Sin_Table ),	
	.SIN_I					( PSD_I1		),			
	.COS_Q					( PSD_Q1		),			
	.Phase_Sensitive_Done	( Detection_complete_clk ) 
);
Phase_Sensitive Phase_Sensitive_2(
 	.rst_n					( sys_rst_n	),
 	.dclk					( PSD2_clk),

   .ADdata					( PSD_AD2	),
 	.cos_rom_data			( PSD_Cos_Table ),	
 	.sin_rom_data			( PSD_Sin_Table ),	
 	.SIN_I					( PSD_I2		),
 	.COS_Q					( PSD_Q2		)	
);
Phase_Sensitive Phase_Sensitive_3(
 	.rst_n					( sys_rst_n	),
 	.dclk					( PSD3_clk),

 	.ADdata					( PSD_AD3	),
 	.cos_rom_data			( PSD_Cos_Table ),	
 	.sin_rom_data			( PSD_Sin_Table ),	
 	.SIN_I					( PSD_I3		),
 	.COS_Q					( PSD_Q3		)		
);
Phase_Sensitive Phase_Sensitive_4(
 	.rst_n					( sys_rst_n	),
 	.dclk					( PSD4_clk),

 	.ADdata					( PSD_AD4	),
 	.cos_rom_data			( PSD_Cos_Table ),	
 	.sin_rom_data			( PSD_Sin_Table ),	
 	.SIN_I					( PSD_I4		),
 	.COS_Q					( PSD_Q4		)	
);
// Phase_Sensitive Phase_Sensitive_5(
// 	.rst_n					( sys_rst_n	),
// 	.dclk					( PSD5_clk),

// 	.ADdata					( PSD_AD5   ),
// 	.cos_rom_data			( PSD_Cos_Table ),	
// 	.sin_rom_data			( PSD_Sin_Table ),	
// 	.SIN_I					( PSD_I5		),
// 	.COS_Q					( PSD_Q5		)		
// );
// Phase_Sensitive Phase_Sensitive_6(
// 	.rst_n					( sys_rst_n	),
// 	.dclk					( PSD6_clk),

// 	.ADdata					( PSD_AD6   ),
// 	.cos_rom_data			( PSD_Cos_Table ),	
// 	.sin_rom_data			( PSD_Sin_Table ),	
// 	.SIN_I					( PSD_I6		),
// 	.COS_Q					( PSD_Q6		)		
// );
// Phase_Sensitive Phase_Sensitive_7(
// 	.rst_n					( sys_rst_n	),
// 	.dclk					( PSD7_clk),

// 	.ADdata					( PSD_AD7   ),
// 	.cos_rom_data			( PSD_Cos_Table ),	
// 	.sin_rom_data			( PSD_Sin_Table ),	
// 	.SIN_I					( PSD_I7		),
// 	.COS_Q					( PSD_Q7		)		
// );

/*******************************频率切换****************************************/
parameter freq_4k	= 16'd1;
parameter freq_10k	= 16'd2;
parameter freq_40k	= 16'd4;
parameter freq_100k	= 16'd8;
parameter freq_400k	= 16'd16;

// always @(posedge DAC7821_CS or negedge sys_rst_n) begin
// 	if(!sys_rst_n)begin
// 	  	DA_SIN_ROM  <= 16'h0;			//复位时DA不输出
// 	end
// 	else begin
// 	 case(freq)
// 		freq_4k:
// 				DA_SIN_ROM  <= DA_sinrom_4k;						
// 		freq_10k:
// 				DA_SIN_ROM  <= DA_sinrom_10k;		
// 		freq_40k:
// 				DA_SIN_ROM  <= DA_sinrom_40k;	
// 		freq_100k:
// 				DA_SIN_ROM  <= DA_sinrom_100k;			
// 		freq_400k:
// 				DA_SIN_ROM  <= DA_sinrom_400k;
// 		default: 
// 				DA_SIN_ROM  <= 16'h0;
// 		endcase
// 	end	
// end


always @(posedge AD4020_dclk or negedge sys_rst_n)	
begin 
    if(!sys_rst_n)
		begin
				PSD_Cos_Table <= 16'h0;
				PSD_Sin_Table <= 16'h0;			
		end		
	 else begin
// 	 case(freq)
// 		freq_4k:
// 			begin 		
				PSD_Cos_Table <= cos_data_4k; 	
				PSD_Sin_Table <= sin_data_4k;
				
				PSD_AD1 <= AD4020_data1;
 				PSD_AD2 <= AD4020_data2;
 				PSD_AD3 <= AD4020_data3;
 				PSD_AD4 <= AD4020_data4;

 				Data_I1 <= PSD_I1;Data_Q1 <= PSD_Q1;
 				Data_I2 <= PSD_I2;Data_Q2 <= PSD_Q2;
 				Data_I3 <= PSD_I3;Data_Q3 <= PSD_Q3;
 				Data_I4 <= PSD_I4; Data_Q4 <= PSD_Q4;
// 				Data_I5 <= 64'd0; Data_Q5 <= 64'd0;
// 				Data_I6 <= 64'd0; Data_Q6 <= 64'd0;
// 				Data_I7 <= 64'd0; Data_Q7 <= 64'd0;
// 				Data_I8 <= 64'd0; Data_Q8 <= 64'd0;
// 				Data_I9 <= PSD_I7; Data_Q9 <= PSD_Q7;
// 			end			
// 		freq_10k:
// 			begin				
// 				PSD_Cos_Table <= cos_data_10k;
// 				PSD_Sin_Table <= sin_data_10k;
				
// 				PSD_AD1 <= AD4020_data1;
// 				PSD_AD2 <= AD4020_data2;
// 				PSD_AD3 <= AD4020_data3;
// 				PSD_AD4 <= AD4020_data4;
// 				PSD_AD5 <= AD4020_data5;
// 				PSD_AD6 <= AD4020_data6;
// 				PSD_AD7 <= AD4020_data9;

// 				Data_I1 <= PSD_I1;Data_Q1 <= PSD_Q1;
// 				Data_I2 <= PSD_I2;Data_Q2 <= PSD_Q2;
// 				Data_I3 <= PSD_I3;Data_Q3 <= PSD_Q3;
// 				Data_I4 <= PSD_I4;Data_Q4 <= PSD_Q4;
// 				Data_I5 <= PSD_I5;Data_Q5 <= PSD_Q5;
// 				Data_I6 <= PSD_I6;Data_Q6 <= PSD_Q6;
// 				Data_I7 <= 64'd0; Data_Q7 <= 64'd0;
// 				Data_I8 <= 64'd0; Data_Q8 <= 64'd0;
// 				Data_I9 <= PSD_I7;Data_Q9 <= PSD_Q7;
// 			end
			
// 		freq_40k:
// 			begin			
// 				PSD_Cos_Table <= cos_data_40k;
// 				PSD_Sin_Table <= sin_data_40k;
				
// 				PSD_AD1 <= AD4020_data1;
// 				PSD_AD2 <= AD4020_data2;
// 				PSD_AD3 <= AD4020_data3;
// 				PSD_AD4 <= AD4020_data4;
// 				PSD_AD5 <= AD4020_data5;
// 				PSD_AD6 <= AD4020_data6;
// 				PSD_AD7 <= AD4020_data9;

// 				Data_I1 <= PSD_I1; Data_Q1 <= PSD_Q1;
// 				Data_I2 <= PSD_I2; Data_Q2 <= PSD_Q2;
// 				Data_I3 <= PSD_I3;Data_Q3 <= PSD_Q3;
// 				Data_I4 <= PSD_I4;Data_Q4 <= PSD_Q4;
// 				Data_I5 <= PSD_I5;Data_Q5 <= PSD_Q5;
// 				Data_I6 <= PSD_I6;Data_Q6 <= PSD_Q6;
// 				Data_I7 <= 64'd0; Data_Q7 <= 64'd0;
// 				Data_I8 <= 64'd0; Data_Q8 <= 64'd0;	
// 				Data_I9 <= PSD_I7;Data_Q9 <= PSD_Q7;
// 			end
			
// 		freq_100k:
// 			begin		
// 				PSD_Cos_Table <= cos_data_100k;
// 				PSD_Sin_Table <= sin_data_100k;
				
// 				PSD_AD1 <= AD4020_data4;
// 				PSD_AD2 <= AD4020_data5;
// 				PSD_AD3 <= AD4020_data6;
// 				PSD_AD4 <= AD4020_data7;
// 				PSD_AD5 <= AD4020_data8;
// 				PSD_AD7 <= AD4020_data9;	

// 				Data_I1 <= 64'd0; Data_Q1 <= 64'd0;
// 				Data_I2 <= 64'd0; Data_Q2 <= 64'd0;
// 				Data_I3 <= 64'd0; Data_Q3 <= 64'd0;
// 				Data_I4 <= PSD_I1;Data_Q4 <= PSD_Q1;
// 				Data_I5 <= PSD_I2;Data_Q5 <= PSD_Q2;
// 				Data_I6 <= PSD_I3;Data_Q6 <= PSD_Q3;
// 				Data_I7 <= PSD_I4;Data_Q7 <= PSD_Q4;
// 				Data_I8 <= PSD_I5;Data_Q8 <= PSD_Q5;
// 				Data_I9 <= PSD_I7;Data_Q9 <= PSD_Q7;
// 			end
			
// 		freq_400k:
// 			begin
// 				PSD_Cos_Table <= cos_data_400k;
// 				PSD_Sin_Table <= sin_data_400k;

// 				PSD_AD1 <= AD4020_data7;
// 				PSD_AD2 <= AD4020_data8;
// 				PSD_AD7 <= AD4020_data9;

// 				Data_I1 <= 64'd0; Data_Q1 <= 64'd0;
// 				Data_I2 <= 64'd0; Data_Q2 <= 64'd0;
// 				Data_I3 <= 64'd0; Data_Q3 <= 64'd0;
// 				Data_I4 <= 64'd0; Data_Q4 <= 64'd0;
// 				Data_I5 <= 64'd0; Data_Q5 <= 64'd0;
// 				Data_I6 <= 64'd0; Data_Q6 <= 64'd0;
// 				Data_I7 <= PSD_I1;Data_Q7 <= PSD_Q1;
// 				Data_I8 <= PSD_I2;Data_Q8 <= PSD_Q2;
// 				Data_I9 <= PSD_I7;Data_Q9 <= PSD_Q7;
// 			end

// 		default: 
// 			begin	
// 				PSD_Cos_Table <= 16'h0;
// 				PSD_Sin_Table <= 16'h0;
				
// 				Data_I1 <= 64'd0;Data_Q1 <= 64'd0;
// 				Data_I2 <= 64'd0;Data_Q2 <= 64'd0;
// 				Data_I3 <= 64'd0;Data_Q3 <= 64'd0;
// 				Data_I4 <= 64'd0;Data_Q4 <= 64'd0;
// 				Data_I5 <= 64'd0;Data_Q5 <= 64'd0;
// 				Data_I6 <= 64'd0;Data_Q6 <= 64'd0;
// 				Data_I7 <= 64'd0;Data_Q7 <= 64'd0;
// 				Data_I8 <= 64'd0;Data_Q8 <= 64'd0;
// 				Data_I9 <= 64'd0;Data_Q9 <= 64'd0;
// 			end
				
		// endcase
	end
end
endmodule
