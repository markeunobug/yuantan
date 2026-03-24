// module ADS9218_init#
// (
//     parameter SPI_CLK_FREQ_MHZ = 50 
// )(
// //系统时钟复位
// 	input 				clk,
// 	input 				rst_n,
	
// //ADS9218接口
// 	output 				SPI_EN,
// 	output				SPI_CS,
// 	output				SPI_SCLK,
// 	output				SPI_MOSI,
// 	input				SPI_MISO,
	
// //状态指示	
// 	output 	reg		    init_done
// );

// //使能ADS9218 SPI配置模块
// assign SPI_EN = 1'b1;

// //SPI驱动
// wire 			spi_wr_start;
// wire 			spi_wr_done;
// reg [47:0] 	    spi_wr_data;
// reg 			spi_wr_en;
// ADS9218_spi_drive ADS9218_spi_drive_u(
//     .clk					(clk),
//     .rst_n				(rst_n),
	
//     .wr_start			(spi_wr_start),
//     .wr_done			(spi_wr_done),

//     .wr_en				(spi_wr_en),
//     .wr_data			(spi_wr_data),

//     .spi_clk			(SPI_SCLK),
//     .spi_csn			(SPI_CS),
//     .spi_sdo			(SPI_MOSI)
// );

// reg	   [ 1:0]	reset_index;
// reg	   [ 7:0]	index;          
// reg	   [ 2:0]	state;
// reg    [31:0] 	delay_cnt;


// //查找当前输出的SPI数据	
// reg    [47:0]	command;				
// always @ (posedge clk)	begin
//     command <= ADS9218_lut(index);
// end
    
// always @ (posedge clk or negedge rst_n)begin
//     if(!rst_n) begin 
//         index       <= 0;       //索引置0
//         init_done   <= 0;       //初始化完成信号置0
//         state       <= 0;       //状态机置0
//         delay_cnt   <= 0;       //延时计数
//         spi_wr_en   <= 0;       //SPI写使能信号置0
//         reset_index <= 0;
//     end
//     else 
//         case (state)
//             3'd0    :   begin
//                             if(delay_cnt != SPI_CLK_FREQ_MHZ * 30000)   //30ms,手册说最少25ms
//                                 delay_cnt <= delay_cnt + 32'd1;
//                             else 
//                                 begin
//                                     delay_cnt   <= 32'd0;
//                                     state       <= 3'd1;
//                                 end
//                         end
                        
//             3'd1    :   begin 
//                             spi_wr_data <= command;
//                             spi_wr_en   <= 1'b1;	
//                             state 		<= 3'd2;
//                         end  

//             3'd2    :   begin
//                             if(spi_wr_start) begin 
//                                 state <= 3'd3;
//                                 spi_wr_en   <= 1'b0;
//                             end
//                         end

//             3'd3    :   begin
//                             if(spi_wr_done) begin 
//                                 state <= 3'd4;
//                             end
//                         end
                        
//             3'd4    :   begin 
//                             if(index == 8'd10) begin		//配置结束
//                                 if(delay_cnt != SPI_CLK_FREQ_MHZ * 30000)   //30ms
//                                     delay_cnt <= delay_cnt+1;
//                                 else begin
//                                     delay_cnt <= 0; 
// 												state <= 3'd6;			//跳出
//                                 end
//                             end
//                             else begin
//                                 state <= 3'd5;
//                                 index <= index + 8'd1;
//                             end
//                         end
                       
//             3'd5    :   begin //状态5、6重复，可以优化为一个
//                             state <= 3'd1;
//                         end
                        
//             3'd6    :   begin 
//                             state <= state;
//                             init_done <= 1'b1;
//                         end
                        
//             default :   state <= 3'd0;
            
//         endcase
// end
	

// function [47:0] ADS9218_lut;    //配置两片adc，每个24位数据
// input [7:0] index;              //输入索引
// reg [47:0] ADS9218_lut_d;       //中间变量
//     begin
//         case(index)
//             8'd0:  ADS9218_lut_d = {24'h00_0001, 24'h00_0000};		 	// BANK0  0x00      SOFT_RESET = 1  复位器件寄存器
//             // 8'd1:  ADS9218_lut_d = {24'h00_0000, 24'h00_0000};			// BANK0  0x00		SPI_MODE = 0 Daisy-chain SPI mode 菊花链配置  SPI_RD_EN = 0 失能寄存器读 SOFT_RESET = 0 正常操作模式
// 			8'd1:  ADS9218_lut_d = {24'h01_0004, 24'h01_0004};			// BANK0  0x01		DAISY_CHAIN_LEN = 1 2 devices in daisy-chain 2个菊花链设备
//             8'd2:  ADS9218_lut_d = {24'h01_0004, 24'h01_0004};			// 配置第二片adc

//             8'd3:  ADS9218_lut_d = {24'h03_0002, 24'h03_0002};			// BANK0  0x03      REG_BANK_SEL = 0x0002 写BANK1
//             8'd4:  ADS9218_lut_d = {24'h03_0002, 24'h03_0002};

//             8'd5:  ADS9218_lut_d = {24'h0D_0082, 24'h0D_0082};			// BANK1  0x0D	    DATA_FORMAT = 0 直接二进制输出 TMP_REG_LD_2 = 0 TMP_REG_LD_1 = 1 正常操作模式
//             8'd6:  ADS9218_lut_d = {24'h12_0000, 24'h12_0000};  		// BANK1  0x12     XOR_EN = 0 关闭异或模式 DATA_WIDTH = 0 20位模式   DATA_LANES = 0 2-lane模式
//             8'd7:  ADS9218_lut_d = {24'hC0_4400, 24'hC0_4400};          // BANK1  0xC0     DCLK_CFG2 = 1 DCLK_CFG4 = 0 DCLK_CFG1 = 1 PD_ADC = 0
//             8'd8:  ADS9218_lut_d = {24'hC1_0809, 24'hC1_0809};          // BANK1  0XC1     PD_REF = 1 使用外部基准 DATA_RATE = 0 DDR模式  DCLK_CFG3 = 9 20-bit 2-lane 模式
// 				//正常工作
// 			8'd9:  ADS9218_lut_d = {24'h13_0000, 24'h13_001A};  	    // BANK1  0x13     关闭0测试模式
//             8'd10:  ADS9218_lut_d = {24'h18_0000, 24'h18_001A};  	    // BANK1  0x18     关闭1测试模式
//             //测试模式-斜坡输出
//             // 8'd8:  ADS9218_lut_d = {24'h13_001A, 24'h13_001A};          // BANK1  0x13     RAMP_INC_CH0 = 1 斜坡模式自增2 TEST_PAT_MOD_CH0 = 2 斜坡输出模式 TEST_PAT_EN_CH0 = 1 使能测试模式
//             // 8'd9:  ADS9218_lut_d = {24'h18_001A, 24'h18_001A};          // BANK1  0x18     RAMP_INC_CH1 = 1 斜坡模式自增2 TEST_PAT_MOD_CH1 = 2 斜坡输出模式 TEST_PAT_EN_CH1 = 1 使能测试模式     

//         endcase   
//         ADS9218_lut = ADS9218_lut_d;            //输出当前输出值
//     end   

// endfunction   
     
  
// endmodule


module ADS9218_init#
(
    parameter SPI_CLK_FREQ_MHZ = 50 
)(
//系统时钟复位
	input 				clk,
	input 				rst_n,
	
//ADS9218接口
	output 				SPI_EN,
	output				SPI_CS,
	output				SPI_SCLK,
	output				SPI_MOSI,
	input				SPI_MISO,
	
//状态指示	
	output 	reg		    init_done
);

//使能ADS9218 SPI配置模块
assign SPI_EN = 1'b1;

//SPI驱动
wire 			spi_wr_start;
wire 			spi_wr_done;
reg [23:0] 	    spi_wr_data;
reg 			spi_wr_en;
ADS9218_spi_drive ADS9218_spi_drive_u(
    .clk					(clk),
    .rst_n				(rst_n),
	
    .wr_start			(spi_wr_start),
    .wr_done			(spi_wr_done),

    .wr_en				(spi_wr_en),
    .wr_data			(spi_wr_data),

    .spi_clk			(SPI_SCLK),
    .spi_csn			(SPI_CS),
    .spi_sdo			(SPI_MOSI)
);

reg	   [ 1:0]	reset_index;
reg	   [ 7:0]	index;          
reg	   [ 2:0]	state;
reg    [31:0] 	delay_cnt;


//查找当前输出的SPI数据	
reg    [23:0]	command;				
always @ (posedge clk)	begin
    command <= ADS9218_lut(index);
end
    
always @ (posedge clk or negedge rst_n)begin
    if(!rst_n) begin 
        index       <= 0;       //索引置0
        init_done   <= 0;       //初始化完成信号置0
        state       <= 0;       //状态机置0
        delay_cnt   <= 0;       //延时计数
        spi_wr_en   <= 0;       //SPI写使能信号置0
        reset_index <= 0;
    end
    else 
        case (state)
            3'd0    :   begin
                            if(delay_cnt != SPI_CLK_FREQ_MHZ * 10000)   //10ms
                                delay_cnt <= delay_cnt + 32'd1;
                            else 
                                begin
                                    delay_cnt   <= 32'd0;
                                    state       <= 3'd1;
                                end
                        end
                        
            3'd1    :   begin 
                            spi_wr_data <= command;
                            spi_wr_en   <= 1'b1;	
                            state 		<= 3'd2;
                        end  

            3'd2    :   begin
                            if(spi_wr_start) begin 
                                state <= 3'd3;
                                spi_wr_en   <= 1'b0;
                            end
                        end

            3'd3    :   begin
                            if(spi_wr_done) begin 
                                state <= 3'd4;
                            end
                        end
                        
            3'd4    :   begin 
                            if(index == 8'd8) begin
                                if(delay_cnt != SPI_CLK_FREQ_MHZ * 10000)   //10ms
                                    delay_cnt <= delay_cnt+1;
                                else begin
                                    delay_cnt <= 0;   
                                    state <= 3'd7;            //跳出     
                                end
                            end
                            else begin
                                state <= 3'd5;
                                index <= index + 8'd1;
                            end
                        end
                       
            3'd5    :   begin 
                            state <= state + 3'd1;
                        end
                        
            3'd6    :   begin 
                            state <= 3'd1;
                        end
                        
            3'd7    :   begin 
                            state <= state;
                            init_done <= 1'b1;
                        end
                        
            default :   state <= 3'd0;
            
        endcase
end
	

function [23:0] ADS9218_lut;
input [7:0] index;              //输入索引
reg [23:0] ADS9218_lut_d;       //中间变量
    begin
        case(index)
            8'd0:  ADS9218_lut_d = {24'h00_0001};		 	// BANK0  0x00      SOFT_RESET = 1  复位器件寄存器
            8'd1:  ADS9218_lut_d = {24'h00_0004};			// BANK0  0x00		SPI_MODE = 1 传统SPI模式  SPI_RD_EN = 0 失能寄存器读 SOFT_RESET = 0 正常操作模式
            8'd2:  ADS9218_lut_d = {24'h03_0002};			// BANK0  0x03      REG_BANK_SEL = 0x0002 写BANK1
            8'd3:  ADS9218_lut_d = {24'h0D_0002};			// BANK1  0x0D	    DATA_FORMAT = 0 直接二进制输出 TMP_REG_LD_2 = 0 TMP_REG_LD_1 = 1 正常操作模式 
            8'd4:  ADS9218_lut_d = {24'h12_0000};  			// BANK1  0x12     XOR_EN = 0 关闭异或模式 DATA_WIDTH = 0 20位模式   DATA_LANES = 0 2-lane模式
            8'd5:  ADS9218_lut_d = {24'hC0_4400};            // BANK1  0xC0     DCLK_CFG2 = 1 DCLK_CFG4 = 0 DCLK_CFG1 = 1 PD_ADC = 0
            8'd6:  ADS9218_lut_d = {24'hC1_0809};            // BANK1  0XC1     PD_REF = 1 使用外部基准 DATA_RATE = 0 DDR模式  DCLK_CFG3 = 9 20-bit 2-lane 模式
				//正常工作模式
			// 	8'd7:  ADS9218_lut_d = {24'h13_0000};  			// BANK1  0x13     关闭0测试模式
            // 8'd8: ADS9218_lut_d = {24'h18_0000};  			// BANK1  0x18     关闭1测试模式
            //测试模式-斜坡输出
          8'd7:  ADS9218_lut_d = {24'h13_001A};          // BANK1  0x13     RAMP_INC_CH0 = 1 斜坡模式自增2 TEST_PAT_MOD_CH0 = 2 斜坡输出模式 TEST_PAT_EN_CH0 = 1 使能测试模式
           8'd8:  ADS9218_lut_d = {24'h18_001A};          // BANK1  0x18     RAMP_INC_CH1 = 1 斜坡模式自增2 TEST_PAT_MOD_CH1 = 2 斜坡输出模式 TEST_PAT_EN_CH1 = 1 使能测试模式         
        endcase   
        ADS9218_lut = ADS9218_lut_d;            //输出当前输出值
    end   

endfunction   
     
  
endmodule