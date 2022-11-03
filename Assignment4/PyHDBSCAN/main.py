import heapq
import math

import pandas as pd


def calculate_the_euclidean_distance(this_x, this_y, that_x, that_y):
    return math.sqrt((this_x - that_x) * (this_x - that_x) + (this_y - that_y) * (this_y - that_y))


# calculate distance and maintain a mean
# the heap has a size of K elements
def calculate_the_core_distance(this_x, this_y, df, k=5):
    h = []
    for row in range(0, 1100):
        that_x = df.iloc[row, 1]
        that_y = df.iloc[row, 2]
        dist = calculate_the_euclidean_distance(this_x, this_y, that_x, that_y)
        if len(h) < k:
            heapq.heappush(h, -dist)  # -2. -5, -100
        else:
            if h[0] > -dist:
                pass
            else:
                heapq.heappop(h)
                heapq.heappush(h, -dist)
    return -h[0]


# generate mutual reachability distance graph, which is the max of {core(a), core(b), d(a,b)}
def generate_mutual_reachability_graph(df):
    result = []
    for i in range(1100):
        print("========== i ==========: ", str(i))
        temp = []
        this_x = df.iloc[i, 1]
        this_y = df.iloc[i, 2]
        for j in range(1100):
            print("   ========== j ==========: ", str(j))
            that_x = df.iloc[j, 1]
            that_y = df.iloc[j, 2]
            core_a = calculate_the_core_distance(this_x, this_y, df)
            print(core_a)
            core_b = calculate_the_core_distance(that_x, that_y, df)
            print(core_b)
            core_euc = calculate_the_euclidean_distance(this_x, this_y, that_x, that_y)
            temp.append(max(core_a, core_b, core_euc))
        print(temp)
        result.append(temp.copy())

    return pd.DataFrame(result)


if __name__ == "__main__":
    df = pd.read_csv('multishapes.csv')

    # print(df)

    # a = df.iloc[0, 1]
    # b = df.iloc[0, 2]
    # print(a)
    # print(b)
    #
    # x = df.iloc[1099, 1]
    # y = df.iloc[1099, 2]
    # print(x, y)

    mydf = generate_mutual_reachability_graph(df)
    print(mydf)
