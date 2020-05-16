//
//  StrassenMatmulComputor.hpp
//  MNN
//
//  Created by MNN on 2019/02/11.
//  Copyright © 2018, Alibaba Group Holding Limited
//

#ifndef StrassenMatmulComputor_hpp
#define StrassenMatmulComputor_hpp

#include <functional>
#include "core/Backend.hpp"
namespace MNN {
class StrassenMatrixComputor {
public:
    StrassenMatrixComputor(Backend* bn, bool multithread, int maxDepth);
    virtual ~StrassenMatrixComputor();

    /*Clear All Command in the Computor*/
    void onReset();

    /*
     It's assume that:
     A is a matrix where each element is a (eP * lP) matrix
     B is a matrix where each element is a (lP * hP) matrix
     inputs[0] is the transpose of A: AT, inputs[1] is the transpose of B: BT
     outputs[0] is the transpose of C: CT

     Let a be one element of A, b be one element of B,
     then a * b = c is a (eP, hP) vector.
     So C is a matrix where each element is a (eP, hP) vector
     */
    ErrorCode onEncode(const std::vector<Tensor*>& inputs, const std::vector<Tensor*>& outputs);

    void onExecute();

protected:
    Backend* backend() const {
        return mBackend;
    }

private:
    class AddTensor;
    ErrorCode _generateMatMul(const Tensor* A, const Tensor* BT, const Tensor* C, int currentDepth);
    ErrorCode _generateTrivalMatMul(const Tensor* A, const Tensor* BT, const Tensor* C);

    std::vector<std::pair<std::function<void(int tId)>, int>> mFunctions;
    std::vector<std::shared_ptr<AddTensor>> mConstTensor;
    int mMaxDepth;
    bool mSupportMultiThread;

    Backend* mBackend;
};
} // namespace MNN

#endif /* StrassenMatmulComputor_hpp */
