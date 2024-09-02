/*
cmdfifo #(
    .LPTRWIDTH()
    ,.DPTRWIDTH()
    ,.WIDTH()
)
test(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.cmdend(cmdend)
    ,.wen(wen)
    ,.din(din)
    ,.full(full)
    ,.ren(ren)
    ,.valid(valid)
    ,.dout(dout)
    ,.empty(empty)
    ,.rx_done(rx_done)
    ,.tx_done(tx_done)
    ,.usedw(usedw)
);
*/
/////no overtime protect
////use outer signal(s) to control write operation
module cmdfifo #(
    parameter       LPTRWIDTH = 4,
    parameter       DPTRWIDTH  = 2,
    parameter       WIDTH = 8
)
(
        input       clk
        ,input      rst_n
        ,input      cmdend
        //write interface
        ,input      wen
        ,input      [WIDTH-1:0]din
        ,output     full
        //read interface
        ,input      ren
        ,output     reg valid
        ,output     reg [WIDTH-1:0]dout
        ,output     empty
        // rx and tx indicator
        ,output     reg rx_done
        ,output     reg tx_done
        ,output     [DPTRWIDTH:0]usedw
);
localparam LENGTH = exp2(LPTRWIDTH);
localparam DEPTH = exp2(DPTRWIDTH);
//generate full signal
assign full = ({wr_ptr_depth[DPTRWIDTH],wr_ptr_depth[DPTRWIDTH-1:0]} == {!rd_ptr_depth[DPTRWIDTH],rd_ptr_depth[DPTRWIDTH-1:0]})? 1'b1:1'b0;
assign empty = (wr_ptr_depth == rd_ptr_depth) ? 1'b1:1'b0;
assign usedw = (wr_ptr_depth[DPTRWIDTH] >= rd_ptr_depth[DPTRWIDTH])?(wr_ptr_depth - rd_ptr_depth):({~wr_ptr_depth[DPTRWIDTH],wr_ptr_depth[DPTRWIDTH-1:0]} - {~(rd_ptr_depth[DPTRWIDTH]),rd_ptr_depth[DPTRWIDTH-1:0]});

//data buffer
reg [WIDTH-1:0]buffer[LENGTH-1:0][DEPTH-1:0];
//data buffer
reg [DPTRWIDTH-1:0]wr_ptr_depth_ture,rd_ptr_depth_ture;
reg [DPTRWIDTH:0]wr_ptr_depth,rd_ptr_depth;
reg [LPTRWIDTH-1:0]wr_ptr_length,rd_ptr_length;
//write buffer control
reg [DPTRWIDTH:0]i,j;
reg overtime_asyn;
wire overtime;
wire [7:0]cmdnum;
reg [7:0]temp[DEPTH-1:0];
assign cmdnum = temp[rd_ptr_depth_ture];
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
            for (i = 0; i<DEPTH ;i=i+1) begin
                temp[i] <= 8'd0;//record received number(s) of cmd
            end
    end
    else begin
        if (wen&&!full&&wr_ptr_length != LENGTH) begin
            buffer[wr_ptr_length][wr_ptr_depth_ture] <= din;
        end
        else begin
            if (tx_done) begin
                temp[rd_ptr_depth_ture] <= 8'd0;
            end
            if (rx_done) begin
                temp[wr_ptr_depth_ture] <= wr_ptr_length;
            end
        end
    end
end
//read buffer control
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        valid <= 1'b0;
    end
    else begin
        if (ren&&!empty) begin
            valid <= 1'b1;
        end
        else begin
            valid <= 1'b0;
        end
    end
end
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        dout <= 8'd0;
    end
    else begin
        if (ren&&!empty) begin
            dout <= buffer[rd_ptr_length][rd_ptr_depth_ture];
        end
        else begin
            dout <= dout;
        end
    end
end
//length write pointer control
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        wr_ptr_length <= 8'd0;
    end
    else begin
            if (rx_done) begin
                wr_ptr_length <= 8'd0;
            end
            else begin
                if (wen&&!full&&wr_ptr_length != LENGTH) begin
                    wr_ptr_length <= wr_ptr_length + 8'd1;
                end
                else begin
                    wr_ptr_length <= wr_ptr_length;
                end
            end
        end
end
//depth write pointer control
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        wr_ptr_depth_ture <= 8'd0;
        wr_ptr_depth <= 9'd0;
    end
    else begin
        if (rx_done) begin
            wr_ptr_depth_ture <= wr_ptr_depth_ture + 8'd1;
            wr_ptr_depth <= wr_ptr_depth + 9'd1;
        end
        else begin
            wr_ptr_depth_ture <= wr_ptr_depth_ture;
            wr_ptr_depth <= wr_ptr_depth;
        end
    end
end
//length read pointer control
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        rd_ptr_length <= 8'd0;
    end
    else begin
        if (tx_done) begin
            rd_ptr_length <= 8'd0;
        end
        else begin
            if (ren&&!empty) begin
                rd_ptr_length <= rd_ptr_length + 8'd1;
            end
            else begin
                rd_ptr_length <= rd_ptr_length;
            end
        end
    end
end
//depth read pointer control
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        rd_ptr_depth_ture <= 8'd0;
        rd_ptr_depth <= 9'd0;
    end
    else begin
        if (tx_done&&!empty) begin
           rd_ptr_depth_ture <= rd_ptr_depth_ture + 8'd1;
           rd_ptr_depth <= rd_ptr_depth + 9'd1; 
        end
        else begin
            rd_ptr_depth_ture <= rd_ptr_depth_ture;
            rd_ptr_depth <= rd_ptr_depth;
        end
    end
end
//tx_done
always @(*) begin
    if (!rst_n) begin
        tx_done = 1'b0;
    end
    else begin
        if ((rd_ptr_length == cmdnum)&&!empty) begin
            tx_done = 1'b1;
        end
        else begin
            tx_done = 1'b0;
        end
    end
end
always @(*) begin
    if (!rst_n) begin
        rx_done = 1'b0;
    end
    else begin
        if (cmdend) begin //超时结束收数 溢出保护
            rx_done = 1'b1;
        end
        else begin
            rx_done = 1'b0;
        end
    end
end
function integer exp2 ;
input   [4:0] DEPTHWIDTH  ;
reg [4:0]temp;
begin
    exp2 = 1;
    for (temp = 0;temp <DEPTHWIDTH ;temp = temp + 1 ) begin
            exp2 = 2*exp2;
    end
end
endfunction
endmodule