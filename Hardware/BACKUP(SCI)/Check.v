/*
Check Ch1(
    .clk()
    ,.rst_n()
    ,.valid()
    ,.rx_data()
    ,.New_instruction()
    ,.HCE()
    ,.PIE()
    ,.ITE()
    ,.CSE()
    ,.LCE()
    ,.ECE()
);
*/
module Check(
    input       clk,
    input       rst_n,
    input       valid,//pulse
    input  [7:0]rx_data,
    input       New_instruction,
    output reg  HCE,
    output reg  PIE,
    output reg  ITE,
    output reg  CSE,
    output reg  LCE,
    output reg  ECE
);
///////////////////////////////////////////
localparam  ST01 = 5'b00001;
localparam  ST02 = 5'b00010;
localparam  ST03 = 5'b00100;
localparam  ST04 = 5'b01000;
localparam  ST05 = 5'b10000;
///////////////////////////////////////////
wire pulse;
reg asyn;
reg [8:0]pkg_length;
///////////////////////////////////////////
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        asyn <= 1'b1;
        LCE <= 1'b0;
    end
    else begin
        if (New_instruction) begin
            asyn <= 1'b1;
        end
        else begin
            asyn <= 1'b0;
        end
        if (ADDR >= pkg_length) begin
            LCE <= 1'b1;
        end
        else begin
            LCE <= 1'b0;
        end
    end
end
///////////////////////////////////////////
//63\13\25 Enquiry cycle error detect
reg [3:0]STATE7;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        ECE <= 1'b0;
        SCN_asyn <= 1'b1;
        STATE7 <=ST01;
    end
    else begin
        case (STATE7)
        ST01    :begin
                ECE    <= 1'b0;
            if (ADDR == 10'd3) begin
                STATE7 <= ST02;
            end
        end 
        ST02    :begin
            case (rx_data)
            8'h63       :begin
                SCN_asyn <= 1'b0;
                STATE7 <= ST03;
            end 
            8'h13       :begin
                SCN_asyn <= 1'b0;
                STATE7 <= ST03;
            end
            8'h25       :begin
                SCN_asyn <= 1'b0;
                STATE7 <= ST03;
            end
            default: begin
                SCN_asyn <= 1'b1;
                STATE7   <= ST01;
            end
            endcase
        end
        ST03    :begin
            if (New_instruction) begin
                STATE7 <= ST04;
            end
            if (SCN_P) begin
                SCN_asyn <= 1'b1;
                STATE7  <= ST01;
            end
        end
        ST04    :begin
            if (ADDR == 10'd3) begin
                STATE7 <= ST05;
            end
        end
        ST05    :begin
            case (rx_data)
            8'h63       :begin
                SCN_asyn <= 1'b1;
                ECE     <= 1'b1;
                STATE7  <= ST01;
            end 
            8'h13       :begin
                SCN_asyn <= 1'b1;
                ECE     <= 1'b1;
                STATE7  <= ST01;
            end
            8'h25       :begin
                SCN_asyn <= 1'b1;
                ECE     <= 1'b1;
                STATE7  <= ST01;
            end
            default: begin
                STATE7   <= ST03;
                SCN_asyn <= 1'b0;
            end
            endcase
        end 
        endcase
    end
end
////////////////////////////////////////////
//Checksum error detect
reg [2:0]STATE8;
reg [15:0]sum;
reg [7:0]Checksum1,Checksum0;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        CSE <= 1'b0;
        STATE8 <= ST01;
        sum <= 16'd00;
        Checksum1 <= 8'b0;
        Checksum0 <= 8'b0;
    end
    else begin
        case (STATE8)
        ST01    :begin
            CSE <= 1'b0;
            Checksum1 <= 8'b0;
            Checksum0 <= 8'b0;
            if (ADDR == 10'd3) begin
            case (rx_data)
                8'h63:begin
                        STATE8 <= ST02;
                        sum <= 16'd04 + 8'h63;
                end 
                8'h87:begin
                        STATE8 <= ST02;
                        sum <= 16'd04 + 8'h87;
                end
                8'h94:begin
                        STATE8 <= ST02;
                        sum <= 16'd04 + 8'h94;
                end
                8'h13:begin
                        STATE8 <= ST02;
                        sum <= 16'd04 + 8'h13;
                end
                8'h25:begin
                        STATE8 <= ST02;
                        sum <= 16'd04 + 8'h25;
                end
                8'h76:begin
                        STATE8 <= ST02;
                        sum <= 16'd00 + 8'h76;
                end
                default :begin 
                        STATE8 <= ST01;
                        sum <= 16'd00;
                end
            endcase
            end
        end 
        ST02    :begin
            if (valid) begin
                sum <= sum + rx_data;
            end
            if (ADDR == pkg_length - 9'd3) begin
                STATE8 <= ST03;
            end
        end
        ST03    :begin
            if (ADDR == pkg_length - 9'd2) begin
                Checksum1 <= rx_data;
            end
            if (ADDR == pkg_length - 9'd1) begin
                Checksum0 <= rx_data;
                STATE8 <= ST04;
            end
        end
        ST04    :begin
            if (Checksum1 == sum [15:8]) begin
                CSE <= 1'b0;
            end
            else begin
                CSE <= 1'b1;
            end
            if (Checksum0 == sum [7:0]) begin
                CSE <= 1'b0;
            end
            else begin
                CSE <= 1'b1;
            end
            STATE8 <= ST01;
        end
        endcase
    end

end
////////////////////////////////////////////
//Head Check Error and Playload Identifier Error
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        HCE <= 1'b0;
        PIE <= 1'b0;
    end
    else begin
        case (ADDR)
        10'd0    :begin
            if (rx_data == 8'hEB) begin
                HCE <= 1'b0;
            end
            else begin
                HCE <= 1'b0;
            end
        end
        10'd1   :begin
            if (rx_data == 8'h90) begin
                HCE <= 1'b0;
            end
            else begin
                HCE <= 1'b0;
            end
        end
        10'd2   :begin
            if ((rx_data == 8'h04)||(rx_data == 8'h00)) begin
                PIE <= 1'b0;
            end
            else begin
                PIE <= 1'b1;
            end
        end
        endcase
    end
end
////////////////////////////////////////////
//Generate pakge length and Instrutions Type Error
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        ITE <= 1'b0;
        pkg_length <= 9'd4;
    end
    else begin
        if (ADDR == 10'd3) begin
            case (rx_data)
               8'h63:begin
                        ITE <= 1'b0;
                        pkg_length <= 9'd53;
                end 
                8'h87:begin
                        ITE <= 1'b0;
                        pkg_length <= 9'd25;
                end
                8'h94:begin
                        ITE <= 1'b0;
                        pkg_length <= 9'd9;
                end
                8'h13:begin
                        ITE <= 1'b0;
                        pkg_length <= 9'd13;
                end
                8'h25:begin
                        ITE <= 1'b0;
                        pkg_length <= 9'd9;
                end
                8'hFF:begin
                        ITE <= 1'b0;
                        pkg_length <= 9'd4;
                end
                8'hF0:begin
                        ITE <= 1'b0;
                        pkg_length <= 9'd4;
                end
                8'h76:begin
                        ITE <= 1'b0;
                        pkg_length <= 9'd268;
                end
                default :begin 
                        ITE <= 1'b1;
                        pkg_length <= 9'd4;
                end
            endcase
        end
    end
end
counter #(
    .RST(511),
    .START(0)
)
c1(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(asyn)
    ,.en(valid)
    ,.cnt(ADDR)
    ,.pulse(pulse)
);
wire SCN_P;//give a pulse every second 
reg SCN_asyn;
counter #(
    .RST(12000000),//1s
    .START(0)
)
c9(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(SCN_asyn)
    ,.en(1'b1)
    ,.pulse(SCN_P)
);

endmodule