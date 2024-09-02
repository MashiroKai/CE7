/*
Send_interval  test(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.INTERVAL(INTERVAL)
    ,.ready(ready)
    ,.ready_waited(ready_waited)
);

*/


module Send_interval(
    input       clk
    ,input      rst_n
    ,input      [31:0]INTERVAL
    ,input      ready
    ,output     ready_waited
);
reg FLAG,asyn;
assign ready_waited = FLAG ? ready : 1'b0;
counter_32b #(
    .START(0)
)
c1(
    .RST(INTERVAL)
    ,.clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(asyn)
    ,.en(1'b1)
    ,.pulse(ALLOWSEND)
);
wire ALLOWSEND;
/////STATE MACHINE
localparam ST0 = 8'b00000001;
localparam ST1 = 8'b00000010;
localparam ST2 = 8'b00000100;
localparam ST3 = 8'b00001000;
localparam ST4 = 8'b00010000;
localparam ST5 = 8'b00100000;
localparam ST6 = 8'b01000000;
localparam ST7 = 8'b10000000;
reg [7:0]CURRENT_STATE;
reg [7:0]NEXT_STATE;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        CURRENT_STATE <= ST0;
    end
    else begin
        CURRENT_STATE <= NEXT_STATE;
    end
end
always @(*) begin
        case (CURRENT_STATE)
        ST0    :begin
            if (ready) begin    //sample ready signal
                NEXT_STATE = ST1;
            end
            else begin
                NEXT_STATE = ST0;
            end
        end
        ST1    :begin
            if (ALLOWSEND) begin //enable counter
                NEXT_STATE = ST2;
            end
            else begin
                NEXT_STATE = ST1;
            end
        end
        ST2    :begin
            if (!ready) begin     //transmit ready
                NEXT_STATE = ST0;
            end
            else begin
                NEXT_STATE = ST2;
            end
        end  
        default: begin
              NEXT_STATE = ST0;
        end
        endcase
end
always @(*) begin
        if (NEXT_STATE == ST1) begin
            asyn = 1'b0;
        end
        else begin
            asyn = 1'b1;
        end
end
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        FLAG <= 1'b0;
    end
    else begin
        if (NEXT_STATE == ST2) begin
            FLAG <= 1'b1;
        end
        else begin
            FLAG <= 1'b0;
        end
    end
end
endmodule