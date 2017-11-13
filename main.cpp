#include <iostream>
#include <queue>
#include <cstring>
#include <assert.h>
//MIN_MAX��������Alpha��beta��֦
using namespace std;

const int MAX_INT = 65535;
const int MIN_INT = -65535;

class ChessBoard{

	short *ptr;

    int is_win()
    {
        return true;
    }
	int how_win(int maxplayer) {
		//�հ�ȫ�£���ʤ����
		ChessBoard *tmp =new ChessBoard();
		(*tmp) = (*this);
		int re=0;
		for(int i=0;i<cols*rows;i++) if(tmp->ptr[i]==0) tmp->ptr[i] = maxplayer;
		int sum = 0;
		for(int i=0;i<rows;i++) {
			sum = 0;
			for(int j=0;j<cols;j++) sum+=tmp->ptr[i*cols+j];
			if(maxplayer==1&&sum==3) re++;
			if(maxplayer==-1&&sum==-3) re++;
		}
		for(int i=0;i<cols;i++) {
			sum = 0;
			for(int j=0;j<rows;j++) sum+=tmp->ptr[j*cols+i];
			if(maxplayer==1&&sum==3) re++;
			if(maxplayer==-1&&sum==-3) re++;
		}
		sum = 0;
		for(int i=0;i<rows;i++)
			sum +=tmp->ptr[i*cols+i];
		if(maxplayer==1&&sum==3) re++;
		if(maxplayer==-1&&sum==-3) re++;
		sum = 0;
		for(int i=0,j=2;i<rows;i++,j--)
			sum +=tmp->ptr[i*cols+j];
		if(maxplayer==1&&sum==3) re++;
		if(maxplayer==-1&&sum==-3) re++;
		delete tmp;
		return re;
	}


public:
	//������ 3*3
	bool max_player;						//maxѡ�ֵ����
	int rows,cols;
	ChessBoard *childs;
	ChessBoard *next_borther;
	ChessBoard *father;
	int value;						//����ֵ
	ChessBoard(int rows=3,int cols=3) {
		this->rows = rows;
		this->cols = cols;
		ptr = new short[rows*cols];
		for(int i=0;i<rows*cols;i++) ptr[i] = 0;
		next_borther = childs = NULL;
	}
	~ChessBoard() {
		this->rows = 0;
		this->cols = 0;
		if(ptr) delete [] ptr;
		ptr = NULL;
	}
	void operator = (ChessBoard &other) {
		//��������
		if(this->rows!=other.rows || this->cols != other.cols ) return ;
		for(int i=0;i<rows*cols;i++) ptr[i] = other.ptr[i];
	}
	short& operator [] (int n) {
		return ptr[n];
	}
	void put(int row,int col,bool max_player) {
		//���������������
		if(ptr[row*cols+col]==0) {
			ptr[row*cols+col] = max_player?1:-1;
			this->max_player = !max_player;
		}
	}
	void put_child(ChessBoard *child) {
		child->next_borther = childs;
		childs = child;
	}
	int where_is_void(int where[]) {
		//�鿴now�пհ�λ�ã�����������n�Լ�λ��where = i*cols+j
		int n=0;
		for(int i=0;i<rows*cols;i++) {
			if(ptr[i] == 0) where[n++] = i;
		}
		return n;
	}

	void show() {
		//cout<<"++++"<<endl;
		for(int i=0;i<rows;i++) {
			for(int j=0;j<cols;j++) {
				char c = '-';
				if(ptr[i*cols+j]==1) c = 'o';
				if(ptr[i*cols+j]==-1) c = 'x';
				cout<<c;
			}
			cout<<endl;
		}
		cout<<"++++"<<endl;
	}
	int who_win() {
		int sum = 0;
		for(int i=0;i<rows;i++) {
			sum = 0;
			for(int j=0;j<cols;j++) sum+=ptr[i*cols+j];
			if(sum==3) return 1;
			if(sum==-3) return -1;
		}
		for(int i=0;i<cols;i++) {
			sum = 0;
			for(int j=0;j<rows;j++) sum+=ptr[i+cols*j];
			if(sum==3) return 1;
			if(sum==-3) return -1;
		}
		sum = 0;
		for(int i=0;i<rows;i++)
			sum +=ptr[i*cols+i];
		if(sum==3) return 1;
		if(sum==-3) return -1;
		sum = 0;
		for(int i=0,j=2;i<rows;i++,j--)
			sum +=ptr[i*cols+j];
		if(sum==3) return 1;
		if(sum==-3) return -1;
		return 0;//ƽ��
	}
	int evaluate() {
		//�ؼ�
		if(who_win()==1) return MAX_INT;
		else if(who_win()==-1) return MIN_INT;
		return (how_win(1) - how_win(-1));
	}

};



void creat_Tree(queue<ChessBoard*> &qu_chess,ChessBoard &now,int layer=4) {
	//�������������
	int size = now.rows*now.cols;
	int where[size];						//row = where/(cols);col = where%(cols)

	qu_chess.push(&now);
	qu_chess.push(NULL);					//��һ�������־

	while(!qu_chess.empty()) {
		ChessBoard *live = qu_chess.front();	//ȡ��һ�ڵ���Ϊ��չ�ڵ�
		qu_chess.pop();
		if(!live) {
			layer--;
			if(0==layer) break;			  	//����layer����˳�
			qu_chess.push(NULL);		  	//��Ϊһ�������־
			live = qu_chess.front();
			qu_chess.pop();
		}
		int n = live->where_is_void(where);		  	//�ɷ�����λ�������
		//cout<<"######"<<n<<endl;
		for(int j=0;j<n;j++) {
			ChessBoard *child = new ChessBoard();
			(*child) = (*live);
			child->put(where[j]/(child->cols),where[j]%(child->cols),(live->max_player));
			child->father = live;
			live->put_child(child);
			if(child->who_win() == 0)//�����ǰ����δ���������Ӵ���չ
                qu_chess.push(child);
            else if(child->who_win() == 1)//����Ѿ���ʤ����������˽ڵ㲻������չ
                child->value = MAX_INT;
            else
                child->value = MIN_INT;
		}
	}
}

void dispose_tree(ChessBoard *node) {

	//�ͷ�node�������� (except node )
	if(node->childs == NULL) return ;
	else {
		ChessBoard *next,*tmp = node->childs;
		while(tmp) {
			dispose_tree(tmp);
			next = tmp->next_borther;
			delete tmp;
			tmp = next;
		}
	}
}

int minmax_recu(ChessBoard *node,int depth,bool max_player) {
	//�ݹ鼫��С�ж�
	if(depth==0 || node->childs==NULL) {		//����ָ����Ȼ��߽ڵ㲻����չ
		return node->evaluate();
	}

	if(max_player) {						   	//�˴β���ΪMAX
		int bestval = MIN_INT;
		ChessBoard *tmp = node->childs;
		while(tmp) {						   	//���к���ȡ���
			tmp->value = minmax_recu(tmp,depth-1,false);
			bestval = max<int>(bestval,tmp->value);
			//cout<<"max_palyer_bestval:"<<bestval<<endl;
			tmp = tmp->next_borther;
		}
		return bestval;
	} else {									//�˴β���ΪMIN
		int bestval = MAX_INT;
		ChessBoard *tmp = node->childs;
		while(tmp) {						   //���к���ȡ��С
			tmp->value = minmax_recu(tmp,depth-1,true);
			bestval = min<int>(bestval,tmp->value);
			//cout<<"min_palyer_bestval:"<<bestval<<endl;
			tmp = tmp->next_borther;
		}
		return bestval;
	}
}

void MAX_MIN_search(ChessBoard &now_node,int &row,int &col) {
	//����С����
	//������ʼ���棬�����Ĳ�������һ���߷�(row,col)

	queue<ChessBoard*> qu_chess;
	//����2��������
	creat_Tree(qu_chess,now_node,2);

	//���򼫴�С�����õ�Ŀ�경��
	now_node.value = minmax_recu(&now_node,2,now_node.max_player);
/*
    ChessBoard *flag = now_node.childs;
    cout<<flag->value<<endl;
    flag->show();
    flag = flag->next_borther;

    while(flag != NULL)
    {
        cout<<flag->value<<endl;
        flag->show();
        flag = flag->next_borther;
    }

	cout<<"now_node.value: "<<now_node.value<<endl;*/
	ChessBoard *tmp = now_node.childs->childs;

	tmp = now_node.childs;
	while(tmp && tmp->value!=now_node.value) {
		tmp = tmp->next_borther;
	}
	for(int i=0;i<now_node.rows*now_node.cols;i++)
		if(now_node[i]!=(*tmp)[i]) {
			row = i/(now_node.cols);col = i%(now_node.cols);
		}

	//����������
	dispose_tree(&now_node);
	now_node.childs = now_node.next_borther = NULL;

}




int main() {
	//����ֻ����һ��Ԥ��
	//ʵ��ģ�����ʵ��������CHessBoard,�ֱ���Ϊ����
	//���Զ����Լ�ΪMAX��������ж���
	//��Σ�����������һ�������ƽ�ֻ������ո���Ӯ����ʱ,
	//����������м䲽���޲ο��ԡ�
	ChessBoard p;
	ChessBoard p2;
	int next_row=0,next_col=0;
	//���ó�̬

	int flag = true;
	int size = p.rows*p.cols;
	int where[size];
	while(p.who_win() == 0 && p.where_is_void(where) > 0 && p2.where_is_void(where) > 0)
    {
        if(flag)
        {
            MAX_MIN_search(p, next_row, next_col);
            p.put(next_row, next_col, true);
            p2.put(next_row, next_col, false);
        }
        else
        {
            cout<<"*"<<endl;
            MAX_MIN_search(p2, next_row, next_col);
            p2.put(next_row, next_col, true);
            p.put(next_row, next_col, false);
            cout<<"##"<<endl;
        }
        flag = !flag;
        cout<<p.value<<endl;
        p.show();
    }
/*
    p.put(0,0,true);
    p.put(1,1,false);
    MAX_MIN_search(p,next_row,next_col);
	p.put(next_row,next_col,true);
	p.show();

	p.put(0,2,false);
    MAX_MIN_search(p,next_row,next_col);
    cout<<"###########################"<<endl;
	p.put(next_row,next_col,true);
	p.show();
	*/



	return 0;
}
