#include "GameLib/Framework.h"
using namespace GameLib;

#include "State.h"
#include "File.h"

//����ԭ��
void MainLoop();

//ȫ�ֱ���
State* gState = 0;

//�û���װ���������ݱ��׳���mainLoop����
namespace GameLib 
{
	void Framework::update() 
	{
		MainLoop();
	}
}

void MainLoop() 
{
	//����ť����������
	if (Framework::instance().isEndRequested()) 
	{
		if (gState) 
		{
			delete gState;
			gState = 0;
		}
		return;
	}
	//��ʼ����һ֡�����Ƶ�һ��״̬����ɡ�
	if (!gState) 
	{
		File file("stageData.txt");
		if (!(file.Data())) 
		{ 
			//û�����ݣ�
			cout << "stage file could not be read." << endl;
			return;
		}
		gState = new State(file.Data(), file.Size());
		//��һ����
		gState->Draw();
		return; //����
	}
	bool cleared = false;
	//��ѭ��
	//������
	if (gState->HasCleared()) 
	{
		cleared = true;
	}
	//��ȡ����
	cout << "a:left s:right w:up z:down. command?" << endl; //����˵��
	char input;
	cin >> input;
	//�����ж�
	if (input == 'q') 
	{
		delete gState;
		gState = 0;
		Framework::instance().requestEnd();
		return;
	}
	//����
	gState->Update(input);
	//����
	gState->Draw();

	if (cleared) 
	{
		//��ף��Ϣ
		cout << "Congratulation! you win." << endl;
		delete gState;
		gState = 0;
	}
}


