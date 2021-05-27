#include "src/Chapter4/Factory.cpp"

int main() {
    {
        /*工厂模式事例*/
        /*利用智能指针保存factory函数返回的结果，方便调用其缺省析构函数*/
        /*避免资源被创建与资源被转换成资源管理对象两个时间点之间的干扰*/
//        auto_ptr<production> proA = Factory::createProduction(TypeA);
//        auto_ptr<production> proB = Factory::createProduction(TypeB);
//        auto_ptr<production> proC = Factory::createProduction(TypeC);
//        proA->show();
//        proB->show();
//        proC->show();
    }

}
