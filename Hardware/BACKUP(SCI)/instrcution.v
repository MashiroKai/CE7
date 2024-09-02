/*
instrcution in1(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.tx_en(tx_en)
    ,.bps_en_rx()
    ,.New_instruction()
    ,.End_instruction()
    ,.BIE()
    ,.RIE()
    ,.IIE()
);
*/
module instrcution(
        input       clk,
        input       rst_n,
        input       tx_en,
        input       bps_en_rx,
        output  reg New_instruction,
        output  reg End_instruction,
        output  reg BIE,//Bytes interval error
        output  reg RIE,//Response interval error
        output  reg IIE //No reply instrcutions interval error
);
localparam  ST01 = 5'b00001;
localparam  ST02 = 5'b00010;
localparam  ST03 = 5'b00100;
localparam  ST04 = 5'b01000;
localparam  ST05 = 5'b10000;
//monitor the change of instrcution flow direction
reg tx_en_r;
reg End_reply,Fee_reply;
wire pos_tx_en,neg_tx_en;
assign pos_tx_en = tx_en&&!tx_en_r;
assign neg_tx_en = !tx_en&&tx_en_r;
always @(posedge clk) begin
    tx_en_r <= tx_en;
end
//detect the posedge and negedge of the bps_en_rx,used as a sign of the start and end of a uart transmission
reg  bps_en_rx_r;
always @(posedge clk) begin
    bps_en_rx_r <= bps_en_rx;
end
wire pos_bps_en_rx,neg_bps_en_rx;
assign pos_bps_en_rx = !bps_en_rx_r&&bps_en_rx;
assign neg_bps_en_rx = !bps_en_rx&&bps_en_rx_r;
//monitor the next reply of fee
reg [1:0]STATE2;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        STATE2 <= ST01;
        Fee_reply <= 1'b0;
    end
    else begin
        case (STATE2)
        ST01   :begin
            if (!tx_en&&bps_en_rx) begin
                Fee_reply <= 1'b1;
                STATE2 <= ST02;
            end
        end 
        ST02   :begin
            Fee_reply <= 1'b0;
            if (tx_en) begin
                STATE2 <= ST01;
            end
        end
        endcase
    end
end
reg [1:0]STATE6;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        End_reply_asyn <= 1'b1;
        STATE6 <= ST01;
        End_reply <= 1'b0;
    end
    else begin
        case (STATE6)
        ST01    :begin
                End_reply_asyn <= 1'b1;
                End_reply   <= 1'b0;
            if (!tx_en &&neg_bps_en_rx) begin
                STATE6 <= ST02;
            end
        end 
        ST02    :begin
            if (!tx_en &&neg_bps_en_rx) begin
                End_reply_asyn <= 1'b1;
            end
            else begin
                End_reply_asyn <= 1'b0;
            end
            if (End_reply_P||pos_tx_en) begin
                End_reply <= 1'b1;
                STATE6 <= ST01;
            end 
        end
        endcase
    end
end
//Judge if it's a new instrution come in
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        New_instruction <= 1'b0;
    end
    else begin
        if (pos_tx_en||Fee_reply) begin
            New_instruction <= 1'b1;
        end
        else begin
            New_instruction <= 1'b0;
        end
    end
end

//Judge the end of a instrcution or reply
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        End_instruction <= 1'b0;
    end
    else begin
        if (neg_tx_en||End_reply) begin
            End_instruction <= 1'b1;
        end
        else begin
            End_instruction <= 1'b1;
        end
    end
end

//Byte interval error detect
reg [2:0]STATE3;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        BIE <= 1'b0;
        STATE3 <= ST01;
        BIC_asyn <= 1'b1;
    end
    else begin
        case (STATE3)
        ST01    :begin
            BIE <= 1'b0;
            if (neg_bps_en_rx) begin
                BIC_asyn <= 1'b0;
                STATE3 <= ST02;
            end
        end 
        ST02    :begin
            if (pos_bps_en_rx) begin
                BIC_asyn <= 1'b1;
                STATE3 <= ST01;
            end
            if (BIC_P) begin
                BIC_asyn <= 1'b1;
                STATE3 <= ST03;
            end
        end
        ST03    :begin
            if (pos_bps_en_rx) begin
                BIE <= 1'b1;
                STATE3 <= ST01;
            end
            if (New_instruction||neg_tx_en) begin
                STATE3 <= ST01;
            end
        end
        endcase
    end
end

//Fee reply interval error detect
reg [2:0]STATE5;
reg ot_asyn,ts_asyn;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        STATE5 <= ST01;
    end
    else begin
        case (STATE5)
        ST01    :begin
                RIE <= 1'b0;
            if (neg_tx_en) begin
                ot_asyn <= 1'b0;
                ts_asyn <= 1'b0;
                STATE5 <= ST02;
            end
        end 
        ST02    :begin
            if (Fee_reply) begin
                ot_asyn <= 1'b1;
                ts_asyn <= 1'b1;
                RIE <= 1'b1;
                STATE5 <= ST01;
            end
            if (Fee_reply_ts) begin
                ot_asyn <= 1'b0;
                ts_asyn <= 1'b1;
                STATE5 <= ST03;
            end
            if (pos_tx_en) begin
                ot_asyn <= 1'b1;
                ts_asyn <= 1'b1;
                STATE5 <= ST01;
            end
        end
        ST03    :begin
            if (Fee_reply) begin
                ot_asyn <= 1'b0;
                ts_asyn <= 1'b1;
                STATE5 <= ST04;
            end
            if (pos_tx_en) begin
                ot_asyn <= 1'b0;
                ts_asyn <= 1'b1;
                STATE5 <= ST01;
            end
            if (Fee_reply_ot) begin
                ot_asyn <= 1'b1;
                ts_asyn <= 1'b1;
                STATE5 <= ST01;
            end
        end
        ST04    :begin
            if (Fee_reply_ot) begin
                ot_asyn <= 1'b1;
                ts_asyn <= 1'b1;
                RIE <= 1'b1;
                STATE5 <= ST01;
            end
            if (End_reply) begin
                ot_asyn <= 1'b1;
                ts_asyn <= 1'b1;
                RIE <= 1'b0;
                STATE5 <= ST01;
            end
        end
        endcase
    end
end

//no reply instrcution interval error detect
reg [2:0]STATE4;
reg IIC_asyn;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        STATE4 <= ST01;
        IIE <= 1'b0;
        IIC_asyn <= 1'b1;
    end
    else begin
        case (STATE4)
        ST01    : begin
            IIE <= 1'b0;
            if (neg_bps_en_rx) begin
                IIC_asyn <= 1'b0;
                STATE4 <= ST02;
            end
        end
        ST02    :begin
            if (New_instruction&&tx_en) begin
                IIC_asyn <= 1'b1;
                STATE4 <= ST01;
                IIE <= 1'b1;
            end
            if (IIC_P) begin
                IIC_asyn <= 1'b1;
                STATE4 <= ST01;
            end
        end
        endcase
    end
end

//FEE End reply counter
wire End_reply_P;
reg End_reply_asyn;
counter #(
    .RST(24000),//2ms
    .START(0)
)
c1(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(End_reply_asyn)
    ,.en(1'b1)
    ,.pulse(End_reply_P)
);
wire BIC_P;
reg BIC_asyn;
counter #(
    .RST(12000),//1ms
    .START(0)
)
c2(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(BIC_asyn)
    ,.en(1'b1)
    ,.pulse(BIC_P)
);

wire Fee_reply_ot;
counter #(
    .RST(600000),//50ms
    .START(0)
)
c3(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(ot_asyn)
    ,.en(1'b1)
    ,.pulse(Fee_reply_ot)
);
wire  Fee_reply_ts;
counter #(
    .RST(240),//20us
    .START(0)
)
c4(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(ts_asyn)
    ,.en(1'b1)
    ,.pulse(Fee_reply_ts)
);
counter #(
    .RST(36000),//3ms
    .START(0)
)
c5(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(IIC_asyn)
    ,.en(1'b1)
    ,.pulse(IIC_P)
);
endmodule