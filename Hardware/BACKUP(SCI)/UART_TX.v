/*
Uart_Tx u1(
	.clk_in(),
	.rst_n_in(),
	.bps_en(),
	.bps_clk(),
	.valid(),
	.tx_data(),
	.rs422_tx(),
	.ready()
);
*/
(
input					clk_in,			//系统时钟
input					rst_n_in,		//系统复位，低有效
output	reg				bps_en,			//发送时钟使能
input					bps_clk,		//发送时钟输入
input					valid,			//data valid , allow to sample
input			[7:0]	tx_data,		//需要发出的数据
output	reg				rs422_tx,		//UART发送输出
output	reg				ready			//发送完成
);
 
reg						rx_bps_en_r;
reg                     verify; 
 
reg				[3:0]	num;
reg				[12:0]	tx_data_r;
reg				[7:0] 	temp;
//根据接收数据的完成，驱动发送数据操作
always @ (posedge clk_in or negedge rst_n_in) begin
	if(!rst_n_in) begin
		bps_en    <= 1'b0;
	end 
	else if(valid)begin
		bps_en <= 1'b1;						
		temp <= tx_data;	
	end else if(num==4'd13) begin	
		bps_en <= 1'b0;	//一次UART发送需要12个时钟信号，然后结束
	end
end

always @(*) begin
	if (!rst_n_in) begin
		ready = 1'b1;
	end
	else if (num == 4'd13) begin
		ready = 1'b1;
	end
	else if (valid) begin
		ready = 1'b0;
	end 
end
always @(posedge clk_in or negedge rst_n_in) begin
    if (!rst_n_in) begin
        verify <= 0;
    end
    else begin
        verify <= ~(^tx_data);// odd
    end
end

always @(posedge clk_in or negedge rst_n_in) begin
    if (!rst_n_in) begin
        tx_data_r <= 13'd1;
    end
    else begin
        tx_data_r <= {1'b1,1'b1,1'b1,verify,temp,1'b0};
    end
end
 
//当处于工作状态中时，按照发送时钟的节拍发送数据
always @ (posedge clk_in or negedge rst_n_in) begin
	if(!rst_n_in) begin
		num <= 4'b0;
		rs422_tx <= 1'b1;
	end else if(bps_en) begin
		if(bps_clk) begin
			num <= num + 1'b1;
			rs422_tx <= tx_data_r[num];
		end else if(num>=4'd13) 
			num <= 4'd0;	
	end
end
 
endmodule