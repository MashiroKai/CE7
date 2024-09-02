module or5(
            input       [4:0]in
            ,output      out 
);
assign out = (in[0]||in[1]||in[2]||in[3]||in[4])?1'b1:1'b0;
endmodule