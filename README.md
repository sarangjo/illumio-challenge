# NatProcessor

*Sarang Joshi*
*sarangj@cs.uw.edu*
*Illumio Coding Challenge November 2017*

## Usage

To run the program, compile `nat.cpp` with your favorite C++ compiler and run the corresponding output program:

```
./nat [nat_file] [flow_file] [output-file]
```

## Design decisions

I spent a considerable amount of time debating the structure of the NAT Processor in terms of how it would store the mappings. After
much deliberation, it seemed to me that for any hashed structure, the FLOW line handler would need to do a maximum of 3 lookups. So to keep
things clean, I decided to go with three separate maps for the situations of (i) IP and port specified, (ii) Only IP specified, and (iii) Only
port specified. This would result in O(1) lookup time for every line of FLOW.

This separate mapping structure also lent itself to support potentially conflicting NAT mappings, due to the generalization caused by
the * wildcard. The specification left this vague, but functionally it seemed that there might be a situation where one could have conflicting
mappings, but with differing scales of the scope. In the case where both IP and port are specified, there is exactly 1 FLOW line that could
correspond to that. The next general structure would have the IP specified but the port wildcarded, since a simple assumption is a given IP
could have 65535 ports. The least general mapping would have the port specified but the IP wildcarded, since we can have only a theoretical
max of 2^32 = 4,294,967,296 possible unique IP addresses, each having the given specific port. This was handled in the `handleFlowLine`
method, which goes in decreasing order of specificity.

## Testing

Primary testing was through test NAT and FLOW files and manual checking of the corresponding OUTPUT file. If I had more time I would like
to write a cleaner test suite to test the various inner functions of the NatProcessor as well as the overall program.
