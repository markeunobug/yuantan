//module ADS9218_Data_Read (
//	//ADS9218 LVDS接口
//	input			FCLK,
//	input			DCLK,
//	input			DOUTA,
//	input			DOUTB,
//
//	// input 			init_done,	//ADS9218初始化完成信号
//	//输出数据
//	output [17:0] data_A,
//	output [17:0] data_B,
//	output [17:0] signed_test_data_r_A
//);
//
//    // 参数（根据模式调整）
//    localparam FRAME_BITS = 24;  // 帧位宽为24位（18位数据 + 6位填充/XOR等）
//	 localparam [7:0]DATA_ADDR = 46; //读取的位置，用于调整时序 23
//
//    // 内部信号
//	 reg [23:0] data_shift_j_A;//添加缓冲区，用于
//    reg [23:0] data_shift_o_A;
//	 
//	 reg [23:0] data_shift_j_B;
//	 reg [23:0] data_shift_o_B;
//	 
//    reg [FRAME_BITS-1:0] shift_reg_a;  // ADC A的移位寄存器
//    reg [FRAME_BITS-1:0] shift_reg_b;  // ADC B的移位寄存器
//    reg [4:0] bit_cnt;                 // 位计数器（最大24）
//    reg fclk_prev;                     // 用于上升沿检测
//    reg capture_en;                    // 帧期间启用捕获
//
//    // 输出寄存器
//    reg [17:0] data_A_reg;
//    reg [17:0] data_B_reg;
//    // 将FCLK同步到DCLK域（防止亚稳态）
//    reg fclk_sync1, fclk_sync2;
//    always @(posedge DCLK) begin
//        fclk_sync1 <= FCLK;
//        fclk_sync2 <= fclk_sync1;
//    end
//
//    // 检测FCLK上升沿（帧开始）
//    wire fclk_rising = (fclk_sync2 == 1) && (fclk_prev == 0);
//
//    // 主捕获逻辑
//    always @(negedge DCLK or posedge FCLK) begin
//        fclk_prev <= fclk_sync2;
//
//        // 在FCLK上升沿，重置计数器并启动捕获
//        //if (fclk_rising) begin
//		  if (FCLK) begin
//				if(~capture_en)begin
//					bit_cnt <= 0;
//					capture_en <= 1;
//					
//					data_shift_o_A <= {data_shift_o_A[22:0], DOUTA};
//					data_shift_o_B <= {data_shift_o_B[22:0], DOUTB};
//				end
//				else begin
//					 if (bit_cnt == FRAME_BITS - 1) begin
//							 capture_en <= 0;
//							 bit_cnt <= 0;
//								 
//					  end
//					  // 如果捕获启用，则移入数据
//					  if (capture_en) begin
//					  
//							data_shift_o_A <= {data_shift_o_A[22:0], DOUTA};
//							data_shift_o_B <= {data_shift_o_B[22:0], DOUTB};
//							
//							bit_cnt <= bit_cnt + 1;
//					  end
//				end
//
//        end
//		  
//		  else begin
//		  
//			  if (bit_cnt == FRAME_BITS - 1) begin
//					
//					 capture_en <= 0;
//					 bit_cnt <= 0;
//						 
//			  end
//
//			  // 如果捕获启用，则移入数据
//			  if (capture_en) begin
//			  
//					data_shift_o_A <= {data_shift_o_A[22:0], DOUTA};
//					data_shift_o_B <= {data_shift_o_B[22:0], DOUTB};
//					
//					bit_cnt <= bit_cnt + 1;
//
//			  end
//			end
//    end
//
//
//    always @(posedge DCLK) begin
//        if (capture_en) begin
//				
//				data_shift_j_A <= {data_shift_j_A[22:0], DOUTA};
//				data_shift_j_B <= {data_shift_j_B[22:0], DOUTB};
//            
//        end
//    end
//	 
//	 wire [47:0] data_shift_A;
//	 wire [47:0] data_shift_B;
//	
//	//拼接数据
//	 assign data_shift_A = { data_shift_o_A[23], data_shift_j_A[23],
//							data_shift_o_A[22], data_shift_j_A[22],
//							data_shift_o_A[21], data_shift_j_A[21],
//							data_shift_o_A[20], data_shift_j_A[20],
//							data_shift_o_A[19], data_shift_j_A[19],
//							data_shift_o_A[18], data_shift_j_A[18],
//							data_shift_o_A[17], data_shift_j_A[17],
//							data_shift_o_A[16], data_shift_j_A[16],
//							data_shift_o_A[15], data_shift_j_A[15],
//							data_shift_o_A[14], data_shift_j_A[14],
//							data_shift_o_A[13], data_shift_j_A[13],
//							data_shift_o_A[12], data_shift_j_A[12],
//							data_shift_o_A[11], data_shift_j_A[11],
//							data_shift_o_A[10], data_shift_j_A[10],
//							data_shift_o_A[9],  data_shift_j_A[9],
//							data_shift_o_A[8],  data_shift_j_A[8],
//							data_shift_o_A[7],  data_shift_j_A[7],
//							data_shift_o_A[6],  data_shift_j_A[6],
//							data_shift_o_A[5],  data_shift_j_A[5],
//							data_shift_o_A[4],  data_shift_j_A[4],
//							data_shift_o_A[3],  data_shift_j_A[3],
//							data_shift_o_A[2],  data_shift_j_A[2],
//							data_shift_o_A[1],  data_shift_j_A[1],
//							data_shift_o_A[0],  data_shift_j_A[0] 
//	 };
//	 
//	  //拼接数据
//	 assign data_shift_B = {  data_shift_o_B[23], data_shift_j_B[23],
//							data_shift_o_B[22], data_shift_j_B[22],
//							data_shift_o_B[21], data_shift_j_B[21],
//							data_shift_o_B[20], data_shift_j_B[20],
//							data_shift_o_B[19], data_shift_j_B[19],
//							data_shift_o_B[18], data_shift_j_B[18],
//							data_shift_o_B[17], data_shift_j_B[17],
//							data_shift_o_B[16], data_shift_j_B[16],
//							data_shift_o_B[15], data_shift_j_B[15],
//							data_shift_o_B[14], data_shift_j_B[14],
//							data_shift_o_B[13], data_shift_j_B[13],
//							data_shift_o_B[12], data_shift_j_B[12],
//							data_shift_o_B[11], data_shift_j_B[11],
//							data_shift_o_B[10], data_shift_j_B[10],
//							data_shift_o_B[9],  data_shift_j_B[9],
//							data_shift_o_B[8],  data_shift_j_B[8],
//							data_shift_o_B[7],  data_shift_j_B[7],
//							data_shift_o_B[6],  data_shift_j_B[6],
//							data_shift_o_B[5],  data_shift_j_B[5],
//							data_shift_o_B[4],  data_shift_j_B[4],
//							data_shift_o_B[3],  data_shift_j_B[3],
//							data_shift_o_B[2],  data_shift_j_B[2],
//							data_shift_o_B[1],  data_shift_j_B[1],
//	};
//	
//	 always @(posedge FCLK) begin
//			data_A_reg <= data_shift_A[DATA_ADDR:DATA_ADDR-17];
//			data_B_reg <= data_shift_B[DATA_ADDR:DATA_ADDR-17];
//	 end
//						
//	 assign data_A = data_A_reg;		
//	 assign data_B = data_B_reg;
//							
//
//endmodule




 module ADS9218_Data_Read (
 	//ADS9218 LVDS接口
 	input			FCLK,
 	input			DCLK,
 	input			DOUTA,
 	input			DOUTB,

 	// input 			init_done,	//ADS9218初始化完成信号
 	//输出数据
 	output [17:0] data_A,
 	output [17:0] data_B,
 	output [17:0] signed_test_data_r_A
 );

 localparam [7:0]DATA_ADDR = 23; //读取的位置，用于调整时序

 /***********************读取A通道数据***********************/
 reg [23:0] data_shift_j_A;//添加缓冲区，用于
 reg [23:0] data_shift_o_A;
 wire [47:0] data_shift_A;
 reg [17:0] test_data_r_A;


 assign signed_test_data_r_A = data_A[17] ? {1'b1,~data_A[16:0]+1} : data_A;	

 wire [17:0] test_data_A;
 //在DCLK的上下边沿锁存数据
 always @(negedge DCLK) begin
 		data_shift_o_A <= {data_shift_o_A[22:0], DOUTA};
 end

 always @(posedge DCLK) begin
 		data_shift_j_A <= {data_shift_j_A[22:0], DOUTA};
 end

 //拼接数据
 assign data_shift_A = { data_shift_o_A[23], data_shift_j_A[23],
 						data_shift_o_A[22], data_shift_j_A[22],
 						data_shift_o_A[21], data_shift_j_A[21],
 						data_shift_o_A[20], data_shift_j_A[20],
 						data_shift_o_A[19], data_shift_j_A[19],
 						data_shift_o_A[18], data_shift_j_A[18],
 						data_shift_o_A[17], data_shift_j_A[17],
 						data_shift_o_A[16], data_shift_j_A[16],
 						data_shift_o_A[15], data_shift_j_A[15],
 						data_shift_o_A[14], data_shift_j_A[14],
 						data_shift_o_A[13], data_shift_j_A[13],
 						data_shift_o_A[12], data_shift_j_A[12],
 						data_shift_o_A[11], data_shift_j_A[11],
 						data_shift_o_A[10], data_shift_j_A[10],
 						data_shift_o_A[9],  data_shift_j_A[9],
 						data_shift_o_A[8],  data_shift_j_A[8],
 						data_shift_o_A[7],  data_shift_j_A[7],
 						data_shift_o_A[6],  data_shift_j_A[6],
 						data_shift_o_A[5],  data_shift_j_A[5],
 						data_shift_o_A[4],  data_shift_j_A[4],
 						data_shift_o_A[3],  data_shift_j_A[3],
 						data_shift_o_A[2],  data_shift_j_A[2],
 						data_shift_o_A[1],  data_shift_j_A[1],
 						data_shift_o_A[0],  data_shift_j_A[0] 
 };
							


 /**************************读取B通道数据*******************/
 reg [23:0] data_shift_j_B;
 reg [23:0] data_shift_o_B;
 wire [47:0] data_shift_B;
 reg [17:0] test_data_r_B;

 //在DCLK的上下边沿锁存数据
 always @(negedge DCLK) begin
 		data_shift_o_B <= {data_shift_o_B[22:0], DOUTB};
 end

 always @(posedge DCLK) begin
 		data_shift_j_B <= {data_shift_j_B[22:0], DOUTB};
 end

 //拼接数据
 assign data_shift_B = {  data_shift_o_B[23], data_shift_j_B[23],
 						data_shift_o_B[22], data_shift_j_B[22],
 						data_shift_o_B[21], data_shift_j_B[21],
 						data_shift_o_B[20], data_shift_j_B[20],
 						data_shift_o_B[19], data_shift_j_B[19],
 						data_shift_o_B[18], data_shift_j_B[18],
 						data_shift_o_B[17], data_shift_j_B[17],
 						data_shift_o_B[16], data_shift_j_B[16],
 						data_shift_o_B[15], data_shift_j_B[15],
 						data_shift_o_B[14], data_shift_j_B[14],
 						data_shift_o_B[13], data_shift_j_B[13],
 						data_shift_o_B[12], data_shift_j_B[12],
 						data_shift_o_B[11], data_shift_j_B[11],
 						data_shift_o_B[10], data_shift_j_B[10],
 						data_shift_o_B[9],  data_shift_j_B[9],
 						data_shift_o_B[8],  data_shift_j_B[8],
 						data_shift_o_B[7],  data_shift_j_B[7],
 						data_shift_o_B[6],  data_shift_j_B[6],
 						data_shift_o_B[5],  data_shift_j_B[5],
 						data_shift_o_B[4],  data_shift_j_B[4],
 						data_shift_o_B[3],  data_shift_j_B[3],
 						data_shift_o_B[2],  data_shift_j_B[2],
 						data_shift_o_B[1],  data_shift_j_B[1],
 						data_shift_o_B[0],  data_shift_j_B[0] 
 };
					
 /***********************拼接数据**********************/	
 always @(posedge FCLK) begin
 		test_data_r_A <= data_shift_A[DATA_ADDR:DATA_ADDR-17];
 		test_data_r_B <= data_shift_B[DATA_ADDR:DATA_ADDR-17];
 end
					
 assign data_A = test_data_r_A;		
 assign data_B = test_data_r_B;
							
endmodule

