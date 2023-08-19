#include<bits/stdc++.h>
using namespace std;
enum NoteType {
    None = 0,
    Normal = 10,
    Critical = 20,
    Sound = 30,
    SoundPurple = 40,
    Flick = 50,
    HoldStart = 80,
    CriticalHoldStart = 81,
    ScratchHoldStart = 82,
    ScratchCriticalHoldStart = 83,
    Hold = 100,
    CriticalHold = 101,
    ScratchHold = 110,
    HoldEighth = 900
};
enum GimmickType {
    JumpScratch = 1,
    OneDirection = 2,
    Split1 = 11,
    Split2 = 12,
    Split3 = 13,
    Split4 = 14,
    Split5 = 15,
    Split6 = 16
};
struct note {
    double startTime;
    double endTime;
    int startBeat;
    int endBeat;
    int type;
    int leftLane;
    int laneLength;
    int gimmickType;
    int scratchLength;
};
int BPM, BPMbeats, BPMx = 8;
double beatToTime(int beat, int bpm) {
    return beat * 30.0 / bpm;
}
// 获取小节数
string get(int beat) {
    string res = to_string(beat / BPMx);
    if (res.size() == 1) res = "00" + res;
    else if (res.size() == 2) res = "0" + res;
    return res;
}
char getHex(int x) {
    if (x < 10) return char('0' + x);
    else return char('a' + x - 10);
}
char getLane(int lane) {
    return getHex(lane + 1);
}
map<string, map<int, pair<int, int> > > chart;
int holdEndBeat[100];
bool used[81920];
int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " [chart] [sus]" << endl;
        return 1;
    } ifstream fin(argv[1]);
    vector<note> notes;
    while (!fin.eof()) {
        char unused = 0;
        note x; fin >> x.startTime >> unused >> x.endTime >> unused >> x.type >> unused >> x.leftLane >> unused >> x.laneLength >> unused;
        string s; fin >> s; x.scratchLength = atoi(s.substr(s.find(",") + 1).c_str());
        s = s.substr(0, s.find(","));
        if (s == "JumpScratch") x.gimmickType = JumpScratch;
        else if (s == "OneDirection") x.gimmickType = OneDirection;
        else if (s == "11") x.gimmickType = Split1;
        else if (s == "12") x.gimmickType = Split2;
        else if (s == "13") x.gimmickType = Split3;
        else if (s == "14") x.gimmickType = Split4;
        else if (s == "15") x.gimmickType = Split5;
        else if (s == "16") x.gimmickType = Split6;
        else x.gimmickType = 0;
        if (x.gimmickType != 0 && x.gimmickType != JumpScratch && x.gimmickType != OneDirection) continue;
        notes.push_back(x);
    }
    // Note排序
    sort(notes.begin(), notes.end(), [](const note& a, const note& b) {
        if (a.startTime == b.startTime) return a.type < b.type;
        return a.startTime < b.startTime;
    });
    double minDragTime = 1e18; map<int, map<int, double> > lastDragTime;
    for (int i = 0; i < notes.size(); i++) {
        if (notes[i].type == HoldStart || notes[i].type == CriticalHoldStart || 
            notes[i].type == ScratchHoldStart || notes[i].type == ScratchCriticalHoldStart)
            lastDragTime[notes[i].leftLane][notes[i].laneLength] = notes[i].startTime;
        if (notes[i].type == HoldEighth) {
            minDragTime = min(minDragTime, notes[i].startTime - lastDragTime[notes[i].leftLane][notes[i].laneLength]);
            lastDragTime[notes[i].leftLane][notes[i].laneLength] = notes[i].startTime;
        }
    }
    // 计算拍子数
    double minDis = 1e18;
    for (int i = 60; i <= 600; i++) {
        double dis = 0;
        for (int j = 0; j < notes.size(); j++) {
            int l = 0, r = 1e6, resBeat = -1;
            // 二分查找 beat
            while (l <= r) {
                int mid = (l + r) >> 1;
                if (beatToTime(mid, i) <= notes[j].startTime) {
                    resBeat = mid;
                    l = mid + 1;
                } else r = mid - 1;
            } if (notes[j].startTime - beatToTime(resBeat, i) < beatToTime(resBeat + 1, i) - notes[j].startTime)
                dis += notes[j].startTime - beatToTime(resBeat, i);
            else dis += beatToTime(resBeat + 1, i) - notes[j].startTime;
        } if (dis < minDis) minDis = dis, BPMbeats = i;
    } cout << BPMbeats << endl;
    // 计算 BPM
    BPM = BPMbeats; BPMx *= (BPMbeats / BPM);
    for (int i = 0; i < notes.size(); i++) {
        // 二分查找 startBeat
        int l = 0, r = 1e6, resBeat = -1;
        while (l <= r) {
            int mid = (l + r) >> 1;
            if (beatToTime(mid, BPMbeats) <= notes[i].startTime) {
                resBeat = mid;
                l = mid + 1;
            } else r = mid - 1;
        } if (notes[i].startTime - beatToTime(resBeat, BPMbeats) < beatToTime(resBeat + 1, BPMbeats) - notes[i].startTime)
            notes[i].startBeat = resBeat;
        else notes[i].startBeat = resBeat + 1;
        // 二分查找 endBeat
        l = 0, r = 1e6, resBeat = -1;
        while (l <= r) {
            int mid = (l + r) >> 1;
            if (beatToTime(mid, BPMbeats) <= notes[i].endTime) {
                resBeat = mid;
                l = mid + 1;
            } else r = mid - 1;
        } if (notes[i].endTime - beatToTime(resBeat, BPMbeats) < beatToTime(resBeat + 1, BPMbeats) - notes[i].endTime)
            notes[i].endBeat = resBeat;
        else notes[i].endBeat = resBeat + 1;
    }
    // 开始转谱
    ofstream fout(argv[2]);
    fout << "This file was generated by MikuMikuWorld 2.3.3" << endl;
    fout << "#TITLE \"\"" << endl;
    fout << "#ARTIST \"\"" << endl;
    fout << "#DESIGNER \"\"" << endl;
    fout << "#WAVEOFFSET 0" << endl << endl;
    fout << "#REQUEST \"ticks_per_beat " << 480 / (BPMbeats / BPM) << "\"" << endl << endl;
    fout << "#00002: 4" << endl << endl;
    fout << "#BPM01: " << BPM << endl;
    fout << "#00008: 01" << endl << endl;
    fout << "#TIL00: \"\"" << endl;
    fout << "#HISPEED 00" << endl;
    fout << "#MEASUREHS 00" << endl << endl;
    // Normal, Critical, Flick, Sound, SoundPurple
    for (int i = 0; i < notes.size(); i++) {
        switch (notes[i].type) {
            case Normal: chart["#" + get(notes[i].startBeat) + "1" + getLane(notes[i].leftLane)][notes[i].startBeat % BPMx] = {1, notes[i].laneLength}; break;
            case Critical: chart["#" + get(notes[i].startBeat) + "1" + getLane(notes[i].leftLane)][notes[i].startBeat % BPMx] = {2, notes[i].laneLength}; break;
            case Flick: {
                chart["#" + get(notes[i].startBeat) + "1" + getLane(notes[i].leftLane)][notes[i].startBeat % BPMx] = {1, notes[i].laneLength};
                chart["#" + get(notes[i].startBeat) + "5" + getLane(notes[i].leftLane)][notes[i].startBeat % BPMx] = {1, notes[i].laneLength}; 
            } break;
            case Sound: chart["#" + get(notes[i].startBeat) + "1" + getLane(notes[i].leftLane)][notes[i].startBeat % BPMx] = {1, notes[i].laneLength}; break;
            case SoundPurple: chart["#" + get(notes[i].startBeat) + "1" + getLane(notes[i].leftLane)][notes[i].startBeat % BPMx] = {2, notes[i].laneLength}; break;
        }
    }
    // Others
    for (int i = 0; i < 100; i++) holdEndBeat[i] = -1;
    for (int i = 0; i < notes.size(); i++) {
        if (notes[i].type != HoldStart && 
            notes[i].type != CriticalHoldStart &&
            notes[i].type != ScratchHoldStart &&
            notes[i].type != ScratchHold &&
            notes[i].type != ScratchCriticalHoldStart) continue; 
        if (notes[i].type == ScratchHold && used[i]) continue;
        int endBeat = 1e9, holdIdInt = 0; char holdId = 0;
        for (int j = 0; j < 100; j++) if (holdEndBeat[j] < notes[i].startBeat){holdId = getHex(j); holdIdInt = j; break;}
        chart["#" + get(notes[i].startBeat) + "3" + getLane(notes[i].leftLane) + holdId][notes[i].startBeat % BPMx] = {1, notes[i].laneLength};
        if (notes[i].type == CriticalHoldStart || notes[i].type == ScratchCriticalHoldStart)
            chart["#" + get(notes[i].startBeat) + "1" + getLane(notes[i].leftLane)][notes[i].startBeat % BPMx] = {2, notes[i].laneLength};
        if (notes[i].type == ScratchHold) {
            chart["#" + get(notes[i].startBeat) + "1" + getLane(notes[i].leftLane)][notes[i].startBeat % BPMx] = {3, notes[i].laneLength}, endBeat = notes[i].endBeat, used[i] = true;
            int leftLane = notes[i].leftLane, laneLength = notes[i].laneLength; holdEndBeat[holdIdInt] = endBeat;
            if (notes[i].scratchLength > 0) {
                laneLength = notes[i].scratchLength;
                chart["#" + get(notes[i].endBeat) + "5" + getLane(leftLane)][notes[i].endBeat % BPMx] = {4, laneLength};
            } else if (notes[i].scratchLength < 0) {
                leftLane = notes[i].leftLane + notes[i].laneLength + notes[i].scratchLength;
                laneLength = -notes[i].scratchLength;
                chart["#" + get(notes[i].endBeat) + "5" + getLane(leftLane)][notes[i].endBeat % BPMx] = {3, laneLength};
            } else chart["#" + get(notes[i].endBeat) + "5" + getLane(leftLane)][notes[i].endBeat % BPMx] = {1, laneLength};
            chart["#" + get(notes[i].endBeat) + "3" + getLane(leftLane) + holdId][notes[i].endBeat % BPMx] = {2, laneLength};
        } 
        for (int j = i + 1; j < notes.size(); j++) {
            if (notes[j].startBeat > endBeat) break;
            if (notes[j].leftLane != notes[i].leftLane || notes[j].laneLength != notes[j].laneLength) continue;
            if (notes[j].type == HoldEighth) {
                chart["#" + get(notes[j].startBeat) + "3" + getLane(notes[j].leftLane) + holdId][notes[j].startBeat % BPMx] = {3, notes[j].laneLength};
                continue;
            }
            // 来找结束点
            if (notes[j].type == Normal && notes[j].type == Critical && notes[j].type == Flick) continue;
            if (notes[i].type == HoldStart && notes[j].type != Hold) continue;
            if (notes[i].type == CriticalHoldStart && notes[j].type != CriticalHold) continue;
            if (notes[i].type == ScratchHoldStart && notes[j].type != ScratchHold) continue;
            if (notes[i].type == ScratchCriticalHoldStart && notes[j].type != ScratchHold) continue;
            if (endBeat == 1e9) {
                endBeat = notes[j].endBeat; holdEndBeat[holdIdInt] = endBeat; used[j] = true;
                if (notes[i].type == HoldStart || notes[i].type == CriticalHoldStart) {
                    chart["#" + get(notes[j].endBeat) + "1" + getLane(notes[j].leftLane)][notes[j].endBeat % BPMx] = {3, notes[j].laneLength};
                    chart["#" + get(notes[j].endBeat) + "3" + getLane(notes[j].leftLane) + holdId][notes[j].endBeat % BPMx] = {2, notes[j].laneLength};
                } else if (notes[i].type == ScratchHoldStart || notes[i].type == ScratchCriticalHoldStart) {
                    int leftLane = notes[j].leftLane, laneLength = notes[j].laneLength;
                    if (notes[j].scratchLength > 0) {
                        laneLength = notes[j].scratchLength;
                        chart["#" + get(notes[j].endBeat) + "5" + getLane(leftLane)][notes[j].endBeat % BPMx] = {4, laneLength};
                    } else if (notes[j].scratchLength < 0) {
                        leftLane = notes[j].leftLane + notes[j].laneLength + notes[j].scratchLength;
                        laneLength = -notes[j].scratchLength;
                        chart["#" + get(notes[j].endBeat) + "5" + getLane(leftLane)][notes[j].endBeat % BPMx] = {3, laneLength};
                    } else chart["#" + get(notes[j].endBeat) + "5" + getLane(leftLane)][notes[j].endBeat % BPMx] = {1, laneLength};
                    chart["#" + get(notes[j].endBeat) + "3" + getLane(leftLane) + holdId][notes[j].endBeat % BPMx] = {2, laneLength};
                }
            }
        }
    }
    // Pack
    for (auto v : chart) {
        fout << v.first << ":";
        for (int i = 0; i < BPMx; i++) {
            fout << v.second[i].first;
            if (v.second[i].second < 10) fout << v.second[i].second;
            else fout << char(v.second[i].second - 10 + 'a');
        } fout << endl;
    } fout.close();
}