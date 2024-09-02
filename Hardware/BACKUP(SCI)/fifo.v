/*
fifo #(
    .WIDTH(),
    .DEPTH()
)
f1(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.valid()
    ,.din()
    ,.load()
    ,.dout()
    ,.fifo_valid()
    ,.full()
    ,.empty()
);
*/
module fifo(
    input            clk
    ,input           rst_n
    ,input              valid
    ,input            [WIDTH-1:0]din
    ,input              load
    ,output     reg   [WIDTH-1:0]dout
    ,output     reg     fifo_valid       
    ,output             full
    ,output             empty
);
parameter   WIDTH = 4'd8;
parameter   DEPTH = 10'd512;

reg     [WIDTH-1:0] buffer [DEPTH-1:0];
reg     [logb2(DEPTH):0]  wr_ptr;
reg     [logb2(DEPTH):0]  rd_ptr;

wire    [logb2(DEPTH)-1:0]  wr_ptr_ture;
wire    [logb2(DEPTH)-1:0]  rd_ptr_ture;

assign wr_ptr_ture = wr_ptr[logb2(DEPTH)-1:0];
assign rd_ptr_ture = rd_ptr[logb2(DEPTH)-1:0];

assign full = (wr_ptr == {~(rd_ptr[logb2(DEPTH)]),rd_ptr[logb2(DEPTH)-1:0]})?1'b1:1'b0;
assign empty = (wr_ptr == rd_ptr)?1'b1:1'b0;

//fifo write opperation
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        wr_ptr <= 'd0;
    end
    else if(!full&&valid) begin
        buffer[wr_ptr_ture] <= din ;
        wr_ptr <= wr_ptr + 1'b1;
    end
end

//fifo read opperation
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        rd_ptr <= 'd0;
    end
    else if(!empty&&load) begin
        dout <= buffer[rd_ptr_ture];
        rd_ptr <= rd_ptr + 1'b1;
        fifo_valid <= 1'b1;
    end else begin
        fifo_valid <= 1'b0;
    end
end

function integer logb2;
    input [31:0]            value;
    reg   [31:0]            tmp;
begin
    tmp = value - 1;
    for (logb2 = 0; tmp > 0; logb2 = logb2 + 1) 
        tmp = tmp >> 1;
end
endfunction

endmodule
