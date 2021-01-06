package section1;

@license(LGPL);
import ned.IdealChannel;
//
// TODO documentation
//
network Network
{
    parameters:
        int n = default(5);
    submodules:
        nodes[n]: Node;
        parent: Parent;
    connections: 
       for i=0..sizeof(nodes)-1, for j=i+1..sizeof(nodes)-1
       {
           nodes[i].outs++ --> IdealChannel --> nodes[j].ins++;
           nodes[i].ins++ <-- IdealChannel <-- nodes[j].outs++;
       }  
       for i=0..sizeof(nodes)-1
       {
        nodes[i].ins++ <-- IdealChannel <-- parent.outs++;
       } 

}

