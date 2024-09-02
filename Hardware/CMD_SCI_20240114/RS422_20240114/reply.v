/*
reply test(
        .clk(clk)
        ,.rst_n(rst_n)
        ,.st(st)
        ,.newreply(newreply)
        ,.wen(wen)
        ,.reply(reply)
);
*/
module reply(
        input           clk
        ,input          rst_n
        ,input          [2:0]st
        ,input          newreply
        ,input          wen // connect to strxctrl
        ,output         reply //error and correct reply also need to generate pulse    
);
localparam ST0 = 6'b000001;
localparam ST1 = 6'b000010;
localparam ST2 = 6'b000100;
localparam ST3 = 6'b001000;
localparam ST4 = 6'b010000;
localparam ST5 = 6'b100000;
reg [5:0]STATE;
wire end_eng,end_reply;
wire overtime;
reg eng_asyn,wait_asyn,overtime_asyn;
reg reply_temp;
assign reply = reply_temp||newreply;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        STATE <= ST0;
    end
    else begin
        case (STATE)
        ST0    :begin
                if (st == 3'b001 && newreply) begin//change to posedge
                    STATE <= ST1;
                end
                else begin
                    STATE <= ST0;
                end
        end
        ST1    :begin
                if (end_eng) begin //end 268 bytes;
                    STATE <= ST2;
                end
                else begin
                    if (newreply||overtime) begin
                        STATE <= ST0;
                    end
                    else begin
                        STATE <= STATE;
                    end
                end
        end
        ST2    :begin
                if (end_reply) begin//wait for 200us ensure that there is not another byte data;
                    STATE <= ST0;
                end
                else begin
                    if (newreply) begin
                        STATE <= ST0;
                    end
                    else begin
                        STATE <= STATE;
                    end
                end
        end 
            default: STATE <= ST0;
        endcase
    end
end
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        eng_asyn <= 1'b1;
    end
    else begin
        if (STATE == ST1) begin
            eng_asyn <= 1'b0;
        end
        else begin
            eng_asyn <= 1'b1;
        end
    end
end
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        wait_asyn <= 1'b1;
    end
    else begin
        if (STATE == ST2) begin
            wait_asyn <= wen;
        end
        else begin
            wait_asyn <= 1'b1;
        end
    end
end
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        overtime_asyn <= 1'b1;
    end
    else begin
        if (STATE == ST1) begin
            overtime_asyn <= wen;
        end
        else begin
            overtime_asyn <= 1'b1;
        end
    end
end
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        reply_temp <= 1'b0;
    end
    else begin
        if (STATE == ST0) begin
            reply_temp <= 1'b0;
        end
        else begin
            reply_temp <= 1'b1;
        end
    end
end
counter #(
    .RST(268),
    .START(0)
)
eng_cnt(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(eng_asyn)
    ,.en(wen)
    ,.pulse(end_eng)
);
counter #(
    .RST(2400),//200us
    .START(0)
)
waitreply_cnt(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(wait_asyn)
    ,.en(1'b1)
    ,.pulse(end_reply)
);
counter #(
    .RST(12000),//1ms
    .START(0)
)
overtime_cnt(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(overtime_asyn)
    ,.en(1'b1)
    ,.pulse(overtime)
);
endmodule