bool adder(int in1, int in2, int threshold, int mask)
{
	
	return ( ( ( (in1+in2) & mask ) >> 2 ) > threshold );
}
