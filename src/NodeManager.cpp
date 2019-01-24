#include "NodeManager.h"
#include <DxLib.h>

NodeManager::NodeManager() {
	flame_cnt = 0;

	//Nodeの領域を動的生成
	for (int column = 0; column < Define::WIN_H; column++) {
		grid[column] = new Node[Define::WIN_W];
	}
	//スタートノード設定
	start = &grid[(int)player.y][(int)player.x];

	//グリッドに座標を指定しゴール地点までの距離を計算
	for (int height = 0; height < Define::WIN_H; height++) {
		for (int width = 0; width < Define::WIN_W; width++) {
			grid[height][width].y = height;
			grid[height][width].x = width;
			grid[height][width].calc_hCost(goal);
		}
	}

	//画面端の座標は最初からクローズにしておく
	//横方向
	for (int hoge = 0; hoge < Define::WIN_W; hoge++) {
		//横方向
		grid[0][hoge].status = Block;
		grid[Define::WIN_H - 1][hoge].status = Block;
	}
	//縦方向
	for (int fuga = 0; fuga < Define::WIN_H; fuga++) {
		grid[fuga][0].status = Block;
		grid[fuga][Define::WIN_W - 1].status = Block;
	}

	//ブロック内の全てのノードのステータスを進入不可にする
	for (int cnt = 0; cnt < BLOCKS; cnt++) {
		for (int column = block[cnt].y; column < block[cnt].y_end; column++) {
			for (int row = block[cnt].x; row < block[cnt].x_end; row++)
				grid[column][row].status = Block;
		}
	}

	//スタートノードをオープンリストへ
	openList.push(start);

	//ルートの格納する配列を動的確保
	root = new vector<Point>;
	player.moveAmount = new vector<Point>;
}

//隣接ノードの移動コスト
float NodeManager::moveCost(int x_diff, int y_diff) {
	//縦横のノードは距離1、斜めならルート2
	return (x_diff == 0 || y_diff == 0) ? (float)1 : sqrtf(2.0);
}

eResult NodeManager::search() {
	float ns;												//子ノードの合計コスト候補
	int loop_cnt = 0;										//探索のループ回数
	int limit = (int)(FREQUENCY * FREQUENCY*Define::PI);			//ループ回数の上限:探索周期に比例させて算出ルートが短くならないようにする
	int child_x, child_y;									//子ノードの座標
	Node* center;											//探索時の親ノード

	clearList();											//前回探索時のノードに関わる状態を全てクリア
	Node* start = &grid[(int)player.y][(int)player.x];		//探索開始時点ではスタートノード
	openList.push(start);

	while (!openList.empty()) {
		//オープンリストtopにゴール座標が来る場合は成功
		if (openList.top()->x == goal.x && openList.top()->y == goal.y) {
			printfDx("探索が完了しました\n");
			getPath(*openList.top());
			player.status = arrival;
			return arrival;
		}
		//探索回数が上限に達したら結果を保存して中断
		if (loop_cnt > limit) {
			getPath(*openList.top());
			//clearList();
			return unReach;
		}

		center = openList.top();	//探索時の中心ノード

		//親ノードをオープンリストからpop
		openList.top()->status = Close;
		closeList.push(openList.pop());

		//周辺の8つを子ノードとしてオープンリストに入れる
		for (int diff_row = -1; diff_row <= 1; diff_row++) {
			for (int diff_column = -1; diff_column <= 1; diff_column++) {
				child_y = center->y + diff_column;
				child_x = center->x + diff_row;
				//中央ノードは処理から除外する
				if (!(diff_row == 0 && diff_column == 0) && child_x < Define::WIN_W && child_y < Define::WIN_H) {
					//子ノードの座標
					Node* child = &grid[center->y + diff_column][center->x + diff_row];
					//親ノードの実コストの確定
					center->g = center->score - center->h;
					//InfluenceMapによる追加コストを計算
					child->i_Cost = calcIM_cost(child);

					//子ノードの合計コストの計算
					ns = center->g + moveCost(diff_row, diff_column) + child->h + child->i_Cost;

					switch (child->status) {
					case None:
						child->status = Open;
						child->parent = center;
						child->score = ns;
						openList.push(child);
						break;
					case Open:
						if (ns < child->score) {
							child->score = ns;
							child->parent = center;

							//コストが変更されたので再ソート
							openList.sortList();
						}
						break;
					case Close:
						if (ns < child->score) {
							child->score = ns;
							child->parent = center;

							//オープンリストに戻す
							child->status = Open;
							openList.push(child);
						}
						break;
					}
				}
			}
		}
 		center = openList.top();
		loop_cnt++;
	}
	printfDx("ルートが見つかりませんでした\n");
	return notFound;
}

void NodeManager::getPath(Node _goal) {
	//探索のたびにメモリを再確保
	delete root;
	root = new vector<Point>;

	//親を遡って経路取得
	while (_goal.parent != nullptr) {
		Point _tmp;
		_tmp.x = _goal.x;
		_tmp.y = _goal.y;
		(*root).push_back(_tmp);
		_goal = *_goal.parent;
	}
	//printfDx("経路を取得・格納しました。\n");
	
	//順序が逆順に入ったままなの戻す
	Point _tmp;
	for (unsigned int cnt = 0; cnt < root->size() / 2; cnt++) {
		_tmp = (*root)[cnt];
		(*root)[cnt] = (*root)[root->size() - cnt - 1];
		(*root)[(*root).size() - cnt - 1] = _tmp;
	}

	//前回探索分のルートをメモリごと破棄し直してから記録する
	delete player.moveAmount;
	player.moveAmount = new vector<Point>;
	//子ノードへ移る移動量を親に記録する
	if ((*root).size() > 2) {
		for (unsigned int cnt = 0; cnt < root->size() - 2; cnt++) {
			_tmp.x = (*root)[cnt + 1].x - (*root)[cnt].x;
			_tmp.y = (*root)[cnt + 1].y - (*root)[cnt].y;
			(*player.moveAmount).push_back(_tmp);
		}
	}
	return;
}

//InfluenceMapによるコスト計算
float NodeManager::calcIM_cost(Node* node) {
	//円形領域と三角形領域の危険度
	float Dci = 0, Dtr = 0;
	const float Ctr2 = tanf(20 * Define::PI / 180);	//三角形領域の角度を決める定数 引数はラジアン表記

	for (int cnt = 0; cnt < HUMAN; cnt++) {
		//人と自機の相対距離の軸成分の距離
		static double dx, dy;
		dx = human[cnt].x - node->x;
		dy = human[cnt].y - node->y;
		//円形領域のコスト
		Dci += Imax * max(0.0, 1 - (pow(dx, 2.0) + pow(dy, 2.0))
			/ (pow((player.radius + human[cnt].radius), 2.0) + pow(Cci, 2.0)));
		//三角形領域のコスト
		static double m = 0.0;	//速度ベクトルに沿う成分
		static double s = 0.0;	//速度ベクトルに直行する成分
		static double vx2vy2 = powf(human[cnt].vx, 2.0) + powf(human[cnt].vy, 2.0);
		m = (human[cnt].vx*dx + human[cnt].vy*dy) / vx2vy2;
		s = (-human[cnt].vy*dx + human[cnt].vx*dy) / vx2vy2;
		Dtr += Imax * max(0, 1 - m / Ctr)*max(0, 1 - abs(s) / m * Ctr2);
	}
	printfDx("Dci = %.1f ,Dtr = %.1f\n", Dci, Dtr);		//IM計算コストをダンプ
	return (float)IMw*max(Dci, Dtr);
}

void NodeManager::update() {
	//フレーム同期タイミングの受け渡し
	flame_cnt++;
	flame_cnt %= FREQUENCY;
	player.flameCnt = flame_cnt;
	
	//動的オブジェクトの位置更新
	player.update(&goal);
	for (int cnt = 0; cnt < HUMAN; cnt++) {
		human[cnt].update();
		if (player.checkHit(human[cnt].x, human[cnt].y)) {
			player.stop();
			human[cnt].stop();
		}
	}
}

//全てを描画
void NodeManager::draw() {
	goal.draw();
	player.draw();
	for (int cnt = 0; cnt < HUMAN; cnt++)
		human[cnt].draw();
	for (int cnt = 0; cnt < BLOCKS; cnt++)
		block[cnt].draw();

	//探索経路のルート表示
	for (unsigned int cnt = 0; cnt < root->size(); cnt++) {
		int x = (*root)[cnt].x;
		int y = (*root)[cnt].y;
		DrawBox(x, y, ++x, ++y, GetColor(255, 255, 255), FALSE);
	}
	//ゴールまでの距離と現在地を表示
	DrawFormatString(100, 80, GetColor(255, 255, 255), "距離：%.1f x:%.0f y:%.0f", player.distance(goal), player.x, player.y);
}

//リスト中のノードを初期状態に戻してポップする
void NodeManager::clearList() {
	Node *tmp;
	while (!openList.empty()) {
		DrawBox(openList.top()->x, openList.top()->y, openList.top()->x + 1, openList.top()->y + 1, GetColor(0, 0, 255), TRUE);	//pop時にリスト中のノードを描画
		tmp = openList.top();
		tmp->score = tmp->g = tmp->i_Cost = 0;
		tmp->status = None;
		tmp->parent = nullptr;
		openList.pop();
	}
	while (!closeList.empty()) {
		tmp = closeList.top();
		tmp->score = tmp->g = tmp->i_Cost = 0;
		tmp->status = None;
		tmp->parent = nullptr;
		closeList.pop();
	}
}