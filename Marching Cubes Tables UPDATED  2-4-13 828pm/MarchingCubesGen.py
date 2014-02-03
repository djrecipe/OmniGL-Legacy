rotation_amount=[0,90,180,-90]

x_corners=[[0,4,5,1],[3,7,6,2]]
y_corners=[[0,1,2,3],[4,5,6,7]]
z_corners=[[1,2,6,5],[0,3,7,4]]

x_edges=[[0,8,4,9],[3,7,5,1],[2,11,6,10]]
y_edges=[[0,1,2,3],[8,9,10,11],[4,5,6,7]]
z_edges=[[9,1,10,5],[0,2,6,4],[8,3,11,7]]

equiv_corners=[0b00000000,0b00010000,0b01010000,
               0b00010001,0b00010100,0b10011000,
               0b01010001,0b01010010,0b10011010,
               0b01010101,0b01011010,0b10011001,
               0b10111000,0b11010001,0b11011000,
               0b11111010,0b10111010,0b11011010]

equiv_edges=[[-1,-1],
             [8,4,7],
             [8,4,7,5,10,6],
             [0,7,3,0,4,7],
             [8,4,7,10,1,2],
             [8,2,3,8,4,2,4,6,2],
             [0,7,3,0,4,7,5,10,6],
             [8,4,7,5,10,6,2,9,0],
             [8,2,3,8,4,2,4,6,2,3,9,0],
             [0,7,3,0,4,7,1,2,6,1,6,5],
             [8,4,7,5,10,6,1,9,0,2,3,11],
             [4,6,0,0,6,2],
             [9,3,8,9,2,3,9,5,2,5,6,2],
             [4,5,0,0,5,10,0,10,3,3,10,11],
             [4,5,8,8,5,10,8,10,3,3,10,2],
             [0,4,8,4,10,8,10,2,8,8,2,3],
             [0,4,8,8,4,5,8,5,6,8,6,2,8,2,3],
             [9,0,1,4,5,8,8,5,10,8,10,3,3,10,2]]

equiv_indices=[0,1,1,3,1,2,3,5,1,3,2,5,3,5,5,11,1,3,2,5,
               4,6,6,13,2,5,7,14,6,12,8,5,1,2,3,5,2,7,5,14,
               4,6,6,12,6,8,13,5,3,5,5,11,6,8,12,5,6,13,8,
               5,9,16,16,3,1,4,2,6,3,6,5,12,2,6,7,8,5,13,14,
               5,2,6,7,8,6,9,8,16,7,8,10,17,8,16,17,15,3,6,5,
               13,5,8,11,5,6,9,8,16,12,16,5,3,5,12,14,5,13,16,
               5,3,8,16,17,15,16,4,15,1,1,2,4,6,2,7,6,8,3,5,6,
               13,5,14,12,5,3,5,6,12,6,8,9,16,5,11,8,5,13,5,
               16,3,2,7,6,8,7,10,8,17,6,8,9,16,8,17,16,15,5,
               14,13,5,8,17,16,15,12,5,16,3,16,15,4,1,3,6,6,9,
               5,8,13,16,5,12,8,16,11,5,5,3,5,13,8,16,12,16,
               16,4,14,5,17,15,5,3,15,1,5,8,12,16,14,17,5,15,
               13,16,16,4,5,15,3,1,11,5,5,3,5,15,3,1,5,3,15,
               1,3,1,1,0]

class LeaveLoop( Exception ):
    pass

def x_rotate(iter_in, arr_in):
    temp_equiv=[]
    temp_equiv.extend(arr_in)
    for itera,items in enumerate(temp_equiv):
        new_indx=None
        new_x=-2
        if items in x_corners[0]:
            new_indx=x_corners[0].index(items)+iter_in
            if new_indx>3:
                new_indx=new_indx-4
            new_x=x_corners[0][new_indx]
        elif items in x_corners[1]:
            new_indx=x_corners[1].index(items)+iter_in
            if new_indx>3:
                new_indx=new_indx-4
            new_x=x_corners[1][new_indx]
        #print("Case #"+str(index)+" | itera #"+str(itera)+" | ix="+str(ix)+": changing "+str(temp_equiv[itera])+" to "+str(new_x))
        temp_equiv[itera]=new_x
    return temp_equiv
def y_rotate(iter_in, arr_in):
    temp_equiv=[]
    temp_equiv.extend(arr_in)
    for itera,items in enumerate(arr_in):
        new_indx=None
        new_y=-2
        if items in y_corners[0]:
            new_indx=y_corners[0].index(items)+iter_in
            if new_indx>3:
                new_indx=new_indx-4
            new_y=y_corners[0][new_indx]
        elif items in y_corners[1]:
            new_indx=y_corners[1].index(items)+iter_in
            if new_indx>3:
                new_indx=new_indx-4
            new_y=y_corners[1][new_indx]
        #print("Case #"+str(index)+" | itera #"+str(itera)+" | ix="+str(ix)+": changing "+str(temp_equiv[itera])+" to "+str(new_x))
        temp_equiv[itera]=new_y
    return temp_equiv
def z_rotate(iter_in,arr_in):
    temp_equiv=[]
    temp_equiv.extend(arr_in)
    for itera,items in enumerate(temp_equiv):
        new_indx=None
        new_z=-2
        if items in z_corners[0]:
            new_indx=z_corners[0].index(items)+iter_in
            if new_indx>3:
                new_indx=new_indx-4
            new_z=z_corners[0][new_indx]
        elif items in z_corners[1]:
            new_indx=z_corners[1].index(items)+iter_in
            if new_indx>3:
                new_indx=new_indx-4
            new_z=z_corners[1][new_indx]
        #print("Case #"+str(index)+" | itera #"+str(itera)+" | ix="+str(ix)+": changing "+str(temp_equiv[itera])+" to "+str(new_x))
        temp_equiv[itera]=new_z
    return temp_equiv
def main():
    global equiv_indices,still_wrong
    #
    inverted_cases=[]
    all_equiv_corners=[]
    for cases in equiv_corners:
        temp_case=cases
        corner_index=0
        these_corners=[]
        while(temp_case!=0):
            if temp_case & 1 ==1:
                these_corners.append(corner_index)
            corner_index=corner_index+1
            temp_case=temp_case>>1
        if len(these_corners)==0:
            these_corners.append(-1)
        all_equiv_corners.append(these_corners)
    #print(str(all_equiv_corners))
    #
    inverted_count=0
    all_corners=[]
    for index,cases in enumerate(equiv_indices):
        curr_index=index
        bit_count=0
        corner_index=0
        these_corners=[]
        while(curr_index!=0):
            if curr_index & 1==1:
                bit_count=bit_count+1
                these_corners.append(corner_index)
            curr_index=curr_index>>1
            corner_index=corner_index+1
        if len(these_corners)==0:
            these_corners.append(-1)
        all_corners.append(these_corners)
        #print(str(bit_count))
        expected_corners=equiv_corners[cases]
        expected_bit_count=0
        while(expected_corners!=0):
            if expected_corners & 1 ==1:
                expected_bit_count=expected_bit_count+1
            expected_corners=expected_corners>>1
        #print(str(expected_bit_count))
        if bit_count!=expected_bit_count:
            #print("Case #"+str(index)+" is an inverse of equivalent case #"+str(cases))
            inverted_cases.append(index)
            inverted_count=inverted_count+1
    #print(str(all_corners))
    #
    still_wrong=False
    accounted_indices=[]
    case_rotations=[]
    all_edges=[]
    for index,cases in enumerate(all_corners):
        this_case=[]
        this_case.extend(cases)
        if index in inverted_cases:
            #print("Skipping known inverted case (#"+str(index)+")")
            case_rotations.append([-1,-1,-1])
            accounted_indices.append(index)
            all_edges.append([-1,-1])
            continue
        equiv_case=all_equiv_corners[equiv_indices[index]]
        if len(this_case)!=len(equiv_case):
            #print("Skipping unknown inverted case (#"+str(index)+")")
            case_rotations.append([-1,-1,-1])
            all_edges.append([-1,-1])
            continue
        #print(str(this_case)+"  |  "+str(equiv_case))
        needs_adjustment=False
        for cindex,corners in enumerate(equiv_case):
            if corners!=this_case[cindex]:
                needs_adjustment=True
                break
        if needs_adjustment:
            still_wrong=False
            try:
                for ix in range(4):
                    for iy in range(4):
                        for iz in range(4):
                            temp_equiv=[]
                            temp_equiv.extend(equiv_case)
                            temp_equiv2=[]
                            temp_equiv2.extend(x_rotate(ix,temp_equiv))
                            #print(str(rotation_amount[iy])+" Y | "+str(equiv_case)+">>>"+str(temp_equiv2))
                            temp_equiv3=[]
                            temp_equiv3.extend(y_rotate(iy,temp_equiv2))
                            temp_equiv=[]
                            temp_equiv.extend(z_rotate(iz,temp_equiv3))
                            #print("temp_equiv: "+str(temp_equiv)+" ... this_case: "+str(this_case))
                            leave_bool=True
                            for cindex,corners in enumerate(temp_equiv):
                                if int(corners) not in this_case:
                                    #print(str(corners)+" vs "+str(this_case[cindex]))
                                    leave_bool=False
                                    if(ix==3 and iy==3 and iz==3):
                                        pass
                                        case_rotations.append([-1,-1,-1])
                                        all_edges.append([-1,-1])
                                        #print("Unaccounted index:"+str(index))
                                    else:
                                        pass
                                        #print("Index: "+str(index)+": Discrepancy Found.")
                                    break
                                    #print("temp_equiv: "+str(temp_equiv)+" vs orig_equiv: "+str(orig_equiv))
                            if leave_bool:
                                case_rotations.append([ix,iy,iz])
                                rotated_edges=[]
                                for edges in equiv_edges[equiv_indices[index]]:
                                    edge_index=-2
                                    if edges==-1:
                                        rotated_edges.extend([-1,-1])
                                        break
                                    for areas in x_edges:
                                        if edges in areas:
                                            edge_index=areas.index(edges)
                                            edge_index=edge_index+ix
                                            if edge_index>3:
                                                edge_index=edge_index-4
                                            rotated_edges.append(areas[edge_index])
                                            break
                                for edgindx,edges in enumerate(rotated_edges):
                                    edge_index=-2
                                    if edges==-1:
                                        break
                                    for areas in y_edges:
                                        if edges in areas:
                                            edge_index=areas.index(edges)
                                            edge_index=edge_index+iy
                                            if edge_index>3:
                                                edge_index=edge_index-4
                                            rotated_edges[edgindx]=areas[edge_index]
                                            break
                                for edgindx,edges in enumerate(rotated_edges):
                                    edge_index=-2
                                    if edges==-1:
                                        break
                                    for areas in z_edges:
                                        if edges in areas:
                                            edge_index=areas.index(edges)
                                            edge_index=edge_index+iz
                                            if edge_index>3:
                                                edge_index=edge_index-4
                                            rotated_edges[edgindx]=areas[edge_index]
                                            break
                                print(str(rotated_edges))
                                raise LeaveLoop
            except LeaveLoop:
                #print("Solved case #"+str(index)+" with: "+str(case_rotations[index]))
                accounted_indices.append(index)
        else:
            accounted_indices.append(index)
            case_rotations.append([-1,-1,-1])
            #print("Case #"+str(index)+" is a perfect match to equivalent case #"+str(equiv_indices[index]))
    unaccounted_count=0
    for i in range(256):
        if i not in accounted_indices:
            #print("Unaccounted index:"+str(i))
            unaccounted_count=unaccounted_count+1
    print("Total unaccounted indices: "+str(unaccounted_count))
main()