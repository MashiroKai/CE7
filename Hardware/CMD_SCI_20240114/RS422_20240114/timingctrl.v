/*
timingctrl test(
        .clk(clk)
        ,.rst_n(rst_n)
        ,.send_done(send_done)
        ,.reply(reply)
        ,.ENABLE(enable)
);

*/

module timingctrl (
        input       clk
        ,input      rst_n
        ,input      tx_en //negedge of tx_en
        ,input      reply
        ,output     reg ENABLE
);
pulsed_reg #(
    .WIDTH(1)
)
p1(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.signal(tx_en)
    ,.neg(send_done)
);
localparam ST0 = 6'b000001;
localparam ST1 = 6'b000010;
localparam ST2 = 6'b000100;
localparam ST3 = 6'b001000;
localparam ST4 = 6'b010000;
localparam ST5 = 6'b100000;
reg [5:0]STATE;
always @(*) begin
    if (!rst_n) begin
        ENABLE = 1'b0;
    end
    else begin
        if (STATE == ST0) begin
            ENABLE = 1'b1;
        end
        else begin
            ENABLE = 1'b0;
        end
    end
end
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        STATE <= ST0;
    end
    else begin
        case (STATE)
        ST0    :begin
            if (tx_en) begin
                STATE <= ST4;
            end
            else begin
                STATE <= STATE;
            end
        end
        ST4    :begin
            if (send_done) begin
                STATE <= ST1;
            end
            else begin
                STATE <= STATE;
            end
        end 
        ST1    :begin
            if (reply) begin
                STATE <= ST3;
            end
            else begin
                if (overtime) begin//nonreply overtime
                    STATE <= ST0;
                end
                else begin
                    STATE <= STATE;
                end
            end
        end
        ST2     :begin
            if (!reply) begin
                STATE <= ST3;
            end
            else begin
                STATE <= STATE;
            end
        end 
        ST3     :begin
            if (waittime) begin
                STATE <= ST0;
            end
            else begin
                STATE <= STATE;
            end
        end
            default: STATE <= ST0;
        endcase
    end
end
reg overtime_en;
always @(*) begin
    if (!rst_n) begin
        overtime_en = 1'b0;
    end
    else begin
        if (STATE == ST1) begin
            overtime_en = 1'b1;
        end
        else begin
            overtime_en = 1'b0;
        end
    end
end
reg waittime_en;
always @(*) begin
    if (!rst_n) begin
        waittime_en = 1'b0;
    end
    else begin
        if (STATE == ST3) begin
            waittime_en = 1'b1;
        end
        else begin
            waittime_en = 1'b0;
        end
    end
end
counter #(
    .RST(12000),//1ms
    .START(0)
)
c1(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(send_done)
    ,.en(overtime_en)
    ,.pulse(overtime)
);
counter #(
    .RST(2500), //  200us
    .START(0)   
)
c2(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(reply)
    ,.en(waittime_en)
    ,.pulse(waittime)
);
endmodule